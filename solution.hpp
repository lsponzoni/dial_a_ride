#if !defined(SOLUTION_HPP)
#define SOLUTION_HPP

#include "route.hpp"
#include "instance.hpp"

struct Solution{
	RouteList routes;

	void add(int at_route_position, int vertex_index, int vehicle_index, Instance &problem);
	void push_back(int vertex_index, int vehicle_index, Instance &problem);
	int back(int v);

	void close(int dst);

	Route getRoute(int vehicle_index);
	Route getBigRoute();
};

namespace SolutionF{
	Solution make(RouteList r);
	Solution start(int sdepot, int vehicles);
	double chain(int link,int unlink1, int unlink2, Instance inst);

	Route to_label_route(const Route route, Requests requests);
	Route to_repeat_request_route(const Route labelRoute, const Requests req);

	double labelRouteFitness(const Route r, const Instance i);
	double mfitness(const Route r, const Instance i);
	double geneFitness(const Route r, const Instance i);
}

#endif // SOLUTION_HPP
