#include "feasibility.hpp"

#include <iostream>
/**
 * Feasibility tests for capacity on a route
 *  and time windows 
 *  
 * These assume unbounded maximum wait time by the users.  
 * 
*/


bool test_capacity(Route &r, IndexType vehicle, Instance &g){
	Seats riders = SeatsF::make(0,0,0,0);
	Requests gr = g.request_table;
	for(int k :r){
		riders.accum(gr.seats(k)); // update seats
		if(!g.fleet.can_give_a_ride(riders, vehicle)) return false;
	}
	return true;
}

void show_capacity(Route &r, Instance &g){
	Seats riders = SeatsF::make(0,0,0,0);
	Requests gr = g.request_table;
	for(int k : r){
		riders.accum(gr.seats(k)); // update seats
		std::cout << '\t'<< k << "\t:";
		riders.print();
		std::cout << std::endl;
	}
}

/* Set D0 = e0, L0 = l0
 * Set L(j) = min{L(j-1) + t(j-1, j) + W, l(j) } v
 * Set A(j) = D(j-1) + t(j-1, j) v 
 * 			D(j) = max (A(j), e(j)) v
 * if A(j) > l(j) infeasible 
 * if A(j)+ W > e(j) then 
 * 	if L(j) < e(j) infeasible 
 *  else
 * 		Set A(j) = e(j) - W, D(j) = e(j)
 * if j < 2n, j++ repeat loop
 */

bool firstPass(Route &r, Requests &gr, DMatrix &gdm, int points,
	Schedule &lower_departure, Schedule &upper_departure, Schedule &arrival, Schedule &waits){
	double W = gr.max_wait(); //maximum tolerable wait time
	for(int it_j = 1; it_j < points; it_j++){ // does not care when arrives at the depot
		int j_1 = r[it_j-1];
		int j = r[it_j];
		
		double travel_time = gdm.dist_points(j_1, j);
		arrival[j] = lower_departure[j_1] + travel_time;	
		
		double late = gr.late(j);
		if(arrival[j] > late) return false;

		double service_time = gr.load_time(j);
		
		double early = gr.early(j);
		
		double upper = upper_departure[j_1] + travel_time + service_time;
		upper_departure[j] = (upper < late)? upper: late; //min
		
		double service_start = (arrival[j] > early)? arrival[j] : early;
		waits[j] = service_start - arrival[j];			
		lower_departure[j] = service_start + service_time; 
		
		//if(waits[j] > W) return false;
		//if(upper_departure[j] < early) return false;
		//else {
		//		arrival[j] = early - W;
		//		lower_departure[j] = early + service_time;
		//} 
	}
	return true;
}
/*
 * Implements Tang changes
 * set W^ = D(2n) - A(2n), j = 2n 
 * loop
 * set D(j-1) = A(j) - t(j-1,j) A(j-1) = max (A(j-1),D(j-1) - W)
 * if(j-1 in pickup)
 * 		set delta = D(delivery(j-1)) - D(pickup(j-1)) - alpha t(pickup(j-1), delivery(j-1))
 * 		if delta > 0 adjustment to Drive Time. 
 * 			if delta > W^ infeasible
 * 			else 
 * 				set D(j-1) = D(j-1) + delta  A(j-1) = max(A(j-1), D(j-1) - W)  W = W - delta
 * 					for(k = succ(pickup) to 2n)
 * 						D(k) = max(D(k), D(k-1),t(k-1,k))
 * 		if D(j) > L(j) infeasible* 		
 *	W = W + (D(j) - A(j))
 * j > 0, j-- 
 */

double ride_time_delta(int pickup, Schedule &table, Requests &gr){
	int delivery = gr.delivery_to(pickup);
	return (table[delivery] - table[pickup]) - gr.ride_time(pickup);
}

bool secondPass(Route &r, Requests &gr, DMatrix &gdm, int points, Schedule &lower_departure, Schedule &upper_departure, Schedule &arrival){
	double wait_balance = lower_departure[points] - arrival[points]; 
	for(int it_j = points - 1; it_j > 0; it_j--){
		int j = r[it_j + 1];
		int j_1 = r[it_j]; 
		
		double travel_time = gdm.dist_points(j,j_1);
		double service_time = gr.load_time(j_1);
		lower_departure[j_1] = arrival[j] - travel_time;
		double arrival_slack = lower_departure[j_1] - (service_time + wait_balance);
		if( arrival[j_1] < arrival_slack) 
			arrival[j_1] = arrival_slack;
		if(gr.isPickup(j_1)){
			double ride_time_diff = ride_time_delta(j_1, lower_departure, gr);

			if(ride_time_diff > 0){
				if(ride_time_diff > wait_balance) 
					return false;
				else {
					lower_departure[j_1] = lower_departure[j_1] + ride_time_diff;
					if(arrival[j_1] <  lower_departure[j_1] - wait_balance) 
						arrival[j_1] = lower_departure[j_1] - wait_balance;
					wait_balance = wait_balance - ride_time_diff;
					for(uint k = succ(r, j_1, it_j + 1); k < r.size(); k++){
						if(lower_departure[r[k]] < lower_departure[r[k-1]] + gdm.dist_points(r[k],r[k-1]))
							lower_departure[r[k]] = lower_departure[r[k-1]] + gdm.dist_points(r[k],r[k-1]);
					}
				}
			}
			if(lower_departure[j] > upper_departure[j]) return false;
		}
		wait_balance += (lower_departure[j] - arrival[j]);
	}
	return true;

}


bool thirdPass(Route &r, Requests &gr, DMatrix &gdm, int points, Schedule &lower_departure, Schedule &arrival){
	for(int it_j = 1; it_j < points - 1; it_j++){
		int j = r[it_j];
		int j_1 = r[it_j-1];
		arrival[j] = lower_departure[j_1] + gdm.dist_points(j, j_1);
		if(lower_departure[j] < arrival[j]) 
			lower_departure[j] = arrival[j];
		if(lower_departure[j] > gr.late(j))
			return false;
		if(gr.isDelivery(j)){
			int pickup = gr.pickup_to(j);
			int delivery = gr.delivery_to(j);
			double ride_time_diff = lower_departure[delivery] - lower_departure[pickup] - gr.ride_time(pickup);
			if(ride_time_diff > 0) 
				return false;
		}
	}

	return true;
/* set j = 1 
 * loop
 * set A(j) = D(j-1)+ t(j-1,j) D(j) = max(D(j), A(j))
 * if D(j) > l(j) infeasible
 * if(j in delivery)
 *   set A = D(delivery(j)) - D(pickup(j)) - alpha * t(delivery(j),pickup(j))
 * 	 if( A > 0) infeasible
 * if(j < 2n) j++ 
 */ 
}
// Aj,Dj Give a feasible schedule
void printS(Route r, Schedule &arrival, Schedule &lower_departure, Schedule &waits){
	for(uint i = 0; i < r.size(); i++){
		std::cout << r[i] <<":\t" << "(w:"<< waits[r[i]] << ",\t" << arrival[r[i]] <<
			'\t'  << lower_departure[r[i]]  << std::endl;
	}
}

bool feasible_schedule(Route &r, Instance &g, 
	Schedule &lower_departure, Schedule &upper_departure, 
	Schedule &arrival,	Schedule &waits)
{
	Requests gr = g.request_table;
	DMatrix gdm = g.dm;
	int points = r.size();

	lower_departure[0] = gr.early(0);
	upper_departure[0] = gr.late(0);
	arrival[0] = gr.early(0);

	// supposing short circuit for &&
	bool f = false;
		if(firstPass(r, gr, gdm, points, lower_departure, upper_departure, arrival, waits)) {
			std::cout << "1pass" << std::endl;
			printS(r,arrival,lower_departure, waits);
			if(secondPass(r, gr, gdm, points, lower_departure, upper_departure, arrival)){
				std::cout << "2pass" << std::endl;
				printS(r,arrival,lower_departure, waits);
				if(thirdPass(r, gr, gdm, points, lower_departure, arrival)){
					std::cout << "3pass" << std::endl;
					printS(r,arrival,lower_departure, waits);
					f = true;
				}
			}
		}
	//printS(r, arrival, lower_departure, waits);
	return f;
}
// First Scheduling 
bool test_feasibility(Route &r, VehicleIndex v, Instance &g){
	int points = g.request_table.size();
	Schedule A(points),D(points),L(points), W(points);
	return test_capacity(r, v, g) && feasible_schedule(r, g,  D, L, A, W);
}

