#include "solution.hpp"

#include <vector>

#include <iostream>
void Solution::add(int at_route_position, int vertex_index, int vehicle_index, Instance &problem){
		auto it_to_position = routes[vehicle_index].begin();
		std::advance(it_to_position, at_route_position);
		routes[vehicle_index].insert(it_to_position, vertex_index);
}

int Solution::back(int v){
	return routes[v].size() - 1; 
}

void Solution::push_back(int vertex_index, int vehicle_index, Instance &problem){
	routes[vehicle_index].push_back(vertex_index);
}

void Solution::close(int dst){
	for(int i = 0; i < routes.size(); i++)
		routes[i].push_back(dst);
}

Route Solution::getRoute(int vehicle_index){
	return routes[vehicle_index];
}

Route Solution::getBigRoute(){
	return mergeRoutes(routes);
}


Solution SolutionF::make(RouteList r){
	Solution s = {r};
	return s;
}

Solution SolutionF::start(int sdepot, int vehicles){
	std::vector<Route> routes(vehicles);
	for(int i = 0; i < vehicles; i++)
		routes[i].push_back(sdepot);
	Solution s = {routes};
	return s;
}

Route SolutionF::to_label_route(const Route route, Requests requests){
	Route lroute;
	std::vector<bool> traveled(requests.request_total);
	for(auto t : traveled){ t = false; } 
	for(int v : route){
		if(traveled[v]){
			lroute.push_back(requests.delivery_to(v));
		} else {
			traveled[v] = true;
			lroute.push_back(v);
		}
	}	
	return lroute;
}

Route SolutionF::to_repeat_request_route(const Route labelRoute, const Requests req){
	Route gRoute;
	for(int v : labelRoute){
		if(req.isDelivery(v)){
			auto request_label = req.pickup_to(v);
			gRoute.push_back(request_label);
		} else {
			gRoute.push_back(v);
		}
	}
	return gRoute;
}


double SolutionF::labelRouteFitness(const Route r, const Instance i){
	// TODO 
	// user experience based on time factor
	DVector v;
	for(int k = 1; k < r.size(); k++){
		v.push_back(i.dm.dist_points(r[k-1], r[k]));
	}
	double routingCost = v.sortSum();
	std::cout << "Routing Cost: " << routingCost << std::endl;
	return routingCost;
}

double SolutionF::chain(int link,int unlink1, int unlink2, Instance inst){
	return inst.dm.dist_points(unlink1, link) - 
		inst.dm.dist_points(unlink1, unlink2) + 
		inst.dm.dist_points(link, unlink2);
}

double SolutionF::mfitness(const Route r, const Instance i){
	return labelRouteFitness(r,i);
}

double SolutionF::geneFitness(const Route r, const Instance i){
	Route l = to_label_route(r, i.request_table);
	return labelRouteFitness(l, i);
}
