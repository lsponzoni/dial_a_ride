#include "route.hpp"
#include <iostream>

void printRoute(const Route r){
	using namespace std;
	for(int i : r)
		cout << i << ' ';
	cout << endl;
}


void printBigRoute(const Route r){
	using namespace std;
	int vehicle = 0;
	for(uint i = 0; i < r.size(); i++){
		if(r[i] == 0 && (i < r.size()  ))// Vk: 
			cout << endl << 'V' << vehicle++ << ':' << ' ';
		cout << r[i] << ' ';	
	}
	cout << endl;
}

void printSplitRoutes(RouteList routes){
	for(uint i = 0; i < routes.size(); i++){
		std::cout << 'V' << i << ':' << ' ';
		printRoute(routes[i]);
		std::cout << std::endl;
	}
}

Route mergeRoutes(RouteList r){
	int ssum = 0;
	for(auto route : r)
		ssum += route.size();
	Route merging;
	merging.reserve( ssum); 
	for(auto route : r)
		merging.insert( merging.end(), route.begin(), route.end() );
	return merging;
}

RouteList splitRoutes(const Route r){
	using namespace std;
	RouteList s; 
	uint i = 0;
	
	while(i < r.size()){
		Route n;
		do {
			n.push_back(r[i]);
			i++;
		} while(i < r.size() && r[i] != 0);
		s.push_back(n);
	}
	return s;
}

int succ(Route r, int node, int fromIndex){
	for(uint i = fromIndex; i > 0; i--){
		if(r[i] == node)
			return i + 1;
	}
	return 1;
}
