#include "sequential_heuristics.hpp"

#include "randomized.hpp"
#include <list>

#include "seats.hpp"
#include "fleet.hpp"
#include "sort_requests.hpp"

//Base sequential construction heuristic
Solution cons_base(Indexes requests, Requests &req, Fleet &vehicles, Instance &context)
{
	auto labels = vehicles.labels();

	std::list<int> not_assigned(labels.begin(), labels.end());
	std::vector<int> assigned;

	Solution solution = SolutionF::start(req.src(), vehicles.size());
	
	for(int request_index : requests)
	{		
		int ride;
		bool found = false;
		Seats seats = req.seats(request_index);
		
		for(uint i = 0; i < assigned.size(); i++)
		{
			ride = assigned[i];
			if(vehicles.can_give_a_ride(seats, ride)){
				found = true; 
				break;
			}
		}
		if(!found)
			for(auto v = not_assigned.begin(); v != not_assigned.end(); v++)
			{
				ride = *v;
				if(vehicles.can_give_a_ride(seats, ride)){
					assigned.push_back(ride);
					not_assigned.erase(v);
					break;
				}
			}
		solution.push_back(request_index, ride, context);
		solution.push_back(req.delivery_to(request_index), ride, context);
	}
			
	solution.close(req.dst());	
	return solution;
}


Solution Heuristic::Sequential::cons_distance(Instance &i){	
	Requests &requests = i.request_table;
	Indexes sortedReq = sort_dist_to_src(requests.pickup_labels(), requests.src(), i.dm);
	return cons_base(sortedReq, requests, i.fleet, i);
}


Solution Heuristic::Sequential::cons_time(Instance &i){
	Requests &requests = i.request_table;
	Indexes sortedReq = sort_pickup_time(requests.pickup_labels(), requests);
	return cons_base(sortedReq, requests, i.fleet, i);
}
