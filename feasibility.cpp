#include "feasibility.hpp"

#include <iostream>

void log(std::string message, double value){
	std::cout << message << '\t' << '\t' << value << std::endl;
}

/**
 * Feasibility tests for capacity on a route
 *  and time windows 
 *  
 * Here I assume unbounded maximum wait time by the users.  
 * 
 * Darp has 5 feasibility issues
 * vehicle capacity does not exceed  
 * 
 * early_i < t_i < late_i (1)
 * t_i + travel(i, j) <= t_j
 * t_(delivery) - t_(pickup) > 0
 * t_(delivery) - t_(pickup) <= ride_time
 * 
 * Except we don't define t_i for schedule directly.
 * We define the bounds that t_i can assume.
 * arrival, the  
 * departure
*/
/*
bool evaluateInsertion(int node, int rp, int rd){
}
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



bool firstPass(Route &r, Requests &gr, DMatrix &gdm, int points,
	Schedule &lower_departure, Schedule &upper_departure, Schedule &arrival, Schedule &waits){
	//  Set D0 = e0, L0 = l0
	// j = 1 if j < 2n, j++ repeat loop
	for(int it_j = 1; it_j <= points; it_j++){ // does not care when arrives at the depot
		int j_1 = r[it_j-1];
		int j = r[it_j];
		
		
		//   Set arrival(j) = D(j-1) + t(j-1, j)
		double travel_time = gdm.dist_points(j_1, j);
		arrival[j] = lower_departure[j_1] + travel_time;	
		
		//   if arrival(j) > l(j) infeasible 
		double late = gr.late(j);
		if(arrival[j] > late) return false;

		double service_time = gr.load_time(j);
		
		double early = gr.early(j);
		//  D(j) = max (arrival(j), e(j)) 		
		double service_start = (arrival[j] > early)? arrival[j] : early;
		lower_departure[j] = service_start + service_time; 
		
		double W = gr.max_wait(j);	
		waits[j] = service_start - arrival[j];			
		//  Set L(j) = min{L(j-1) + t(j-1, j) + W, l(j) } 		
		double upper = upper_departure[j_1] + travel_time + W; // W covers service_time
		upper_departure[j] = (upper < late)? upper: late; //min

		//   if arrival(j)+ W < e(j) then 
		if(arrival[j] + W < early){
			// if L(j) < e(j) infeasible 
			if(upper_departure[j] < early) return false;
			else {				
				arrival[j] = early - W;
				waits[j] = W;
				lower_departure[j] = early + service_time;
				log("________Changes", arrival[j]);
			}		
		}
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

double ride_time_delta(int pickup, Schedule &departure, Schedule &arrival, Requests &gr){
	int delivery = gr.delivery_to(pickup);
	return (arrival[delivery] - departure[pickup]) - gr.ride_time(pickup);
}

// Fits to solve the ride times
bool secondPass(Route &r, 
	Requests &gr, DMatrix &gdm, int points, 
	Schedule &lower_departure, Schedule &upper_departure, Schedule &arrival, Schedule &waits)
{
	double wait_balance = lower_departure[points - 1] - arrival[points-1]; 
	//  set W^ = D(2n) - A(2n), j = 2n 
	for(int it_j = points - 1; it_j >= 0; it_j--){
		int jp1 = r[it_j + 1]; //j+1 
		int j =  r[it_j]; // j
		
		//  set D(j-1) = A(j) - t(j-1,j)
		double travel_time = gdm.dist_points(j, jp1);
		lower_departure[j] = arrival[jp1] - travel_time;

		double W = gr.max_wait(j);
		
		//  if(j-1 in pickup)
		if(gr.isPickup(j)){
			//	set delta = D(delivery(j-1)) - D(pickup(j-1)) - max_ride_time(j-1))
			double rt_delta = ride_time_delta(j, lower_departure, arrival, gr);

			//	if delta > 0 adjustment to Drive Time. 	
			if(rt_delta > 0.0f){
				// if delta > W^ infeasible
				if(rt_delta > wait_balance) { 
					log("r:", it_j);
					log("With", j);
					log("Infeasible by delta ", rt_delta);
					log("> wait_balance", wait_balance);
				 	return false;
				} else {

					// set D(j-1) = D(j-1) + delta 
					lower_departure[j] = lower_departure[j] + rt_delta;
					// 	   W = W - delta
					wait_balance = wait_balance - rt_delta;
				
					for(uint it_k = it_j + 1; it_k < r.size(); it_k++){
						int k = r[it_k];
						int k_1 = r[it_k - 1];
						// D(k) = max(D(k), D(k-1),t(k-1,k))
						double lower = (lower_departure[k_1] + gdm.dist_points(k_1, k)/* + gr.load_time(k)*/);
						if(lower_departure[k] < lower)
							lower_departure[k] = lower;
						
					}
				}
			}
			// if D(j) > L(j) infeasible 		
			if(lower_departure[j] > upper_departure[j]){ 
				log("r:", it_j);
				log("With", j);
				log("Infeasible by lower", lower_departure[j]);
				log("> upper", upper_departure[j]);
				return false;
			}
		}

		//	A(j-1) = max (A(j-1),D(j-1) - W)
		if(arrival[j] < (lower_departure[j] - W))
			arrival[j] = lower_departure[j] - W;
		// W = W + (D(j) - A(j))
		waits[j] = (lower_departure[j] - arrival[j]);
		wait_balance = wait_balance + waits[j];	
	}
	return true;

}

void check(int pickup, Schedule &arrival, Schedule &lower_departure, Requests &gr){
	log("point", pickup);				
	log("a", arrival[pickup]);				
	log("d:", lower_departure[pickup]);
	log("e:", gr.early(pickup));
	log("l:", gr.late(pickup));
}

bool thirdPass(Route &r, Requests &gr, DMatrix &gdm, int points, Schedule &lower_departure, Schedule &arrival){
	//   set j = 1 loop if(j < 2n) j++ 
	for(int it_j = 1; it_j < points - 1; it_j++){
		int j = r[it_j];
		int j_1 = r[it_j-1];
		// set A(j) = D(j-1)+ t(j-1,j)
		arrival[j] = lower_departure[j_1] + gdm.dist_points(j, j_1);
		// D(j) = max(D(j), A(j))
		if(lower_departure[j] < arrival[j] /*+ gr.load_time(j)*/){ 
			lower_departure[j] = arrival[j]/* + gr.load_time(j)*/;
			log("Changes TO", j);
		}
		//   if D(j) > l(j) infeasible
		if(lower_departure[j] > gr.late(j)){
			log("Failed departure vs late", j);
			return false;
		}
		//   if(j in delivery)
		//     set delta = D(delivery(j)) - D(pickup(j)) - alpha * t(delivery(j),pickup(j))
		//   	 if( delta > 0) infeasible
		if(gr.isDelivery(j)){
			int pickup = gr.pickup_to(j);
			double rt_delta = ride_time_delta(gr.pickup_to(j), lower_departure, arrival, gr);
			if(rt_delta > 0.0f) {
				log("Ride time", pickup);
				log("rt", rt_delta);

				check(pickup, arrival, lower_departure, gr);
				int it_k = find(r, pickup, it_j);
				check(r[it_k + 1], arrival, lower_departure, gr);
				log("Dist >", gdm.dist_points(pickup, r[it_k + 1]));
					
				check(j, arrival, lower_departure, gr);
				check(r[it_j - 1], arrival, lower_departure, gr);
				log("Dist >", gdm.dist_points(j, r[it_j-1]));

				return false;
			}
		}
	}
	return true;
}
// Aj,Dj Give a feasible schedule
void printS(Route r, Schedule &arrival, Schedule &lower_departure, Schedule &waits){
	for(uint i = 0; i < r.size(); i++){
		std::cout << r[i] <<":\t" << "(w:"<< waits[r[i]] << ", a:" << arrival[r[i]] <<
			", d:"  << lower_departure[r[i]]  << ')'<< std::endl;
	}
}

bool feasible_schedule(Route &r, Instance &g, 
	Schedule &lower_departure, Schedule &upper_departure, 
	Schedule &arrival,	Schedule &waits)
{
	Requests gr = g.request_table;
	DMatrix gdm = g.dm;
	int points = r.size();

	lower_departure[0] = gr.early(1);
	upper_departure[0] = gr.late(1);
	arrival[0] = gr.early(1);
	// supposing short circuit for &&
	bool f = false;
		if(firstPass(r, gr, gdm, points, lower_departure, upper_departure, arrival, waits)) {
			std::cout << "1pass" << std::endl;
			printS(r,arrival,lower_departure, waits);
			if(secondPass(r, gr, gdm, points, lower_departure, upper_departure, arrival, waits)){
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
	g.request_table._ride_time[0] = g.fleet.max_drive_time[v];
	return test_capacity(r, v, g) && feasible_schedule(r, g,  D, L, A, W);
}

void printScheduleConstraints(Route r, Instance &g){
	using namespace std;
	
	auto req = g.request_table;
	for(int i = 0; i < r.size(); i++){
		cout << "var x" << r[i] << ">=" << req.early(r[i]) << ';' << endl;
	}
	cout << "minimize obj: x0";
	for(int i = 1; i < r.size(); i++)
		cout << "+ x" << r[i] ;
	cout << ';' << endl;
	int rule = 0;
	for(int i = 0; i < r.size(); i++){
		//s.t. RN: xi <= #late;
		cout << "s.t. R" << rule++ << ": "; 
		cout << "x" << r[i] << " <= " << req.late(r[i]) << ';' << endl;
	}
	
	for(int i = 1; i < r.size(); i++){
		//s.t. RN: xi-1 + t(i,i-1) <= xi;
		cout << "s.t. R" << rule++ << ": ";
		cout << "x" << r[i- 1] << "+ " << g.dm.dist_points(r[i-1], r[i]) << " <= " << "x" << r[i] <<  ';' << endl;
	}

	//s.t. RN: x(delivery) - x(pickup) <= ride_time;
	for(int i = 1; i < r.size(); i++){
		if(!req.isPickup(r[i]))
			continue;
		cout << "s.t. R" << rule++ << ": ";
		cout << "x" << req.delivery_to(r[i]) <<  "- " <<
			 "x" << r[i] << " <= " << req.ride_time(i) <<  ';' << endl;
	}
	cout <<	"solve;\n" << "display: x0";
	for(int i = 1; i < r.size(); i++)
		cout << ", x" << r[i] ;
	cout << ";" << endl << "end;\n";
}