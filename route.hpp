
#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <vector>

typedef std::vector<int> Route;
typedef std::vector<Route> RouteList;

int succ(Route r, int node, int from);

void printSplitRoutes(RouteList routes);

void printRoute(const Route r);
void printBigRoute(const Route r);
Route mergeRoutes(RouteList r);
RouteList splitRoutes(const Route r);


#endif