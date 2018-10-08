#include "darp.hpp"
#include "feasibility.hpp"

int main(){
	using namespace std;

	Instance s = InstanceF::read();

	Solution rl1 = Heuristic::Sequential::cons_distance(s);
	Route sch1 = mergeRoutes(rl1.routes);
	printBigRoute(sch1);
	SolutionF::labelRouteFitness(sch1, s);

	Solution rl2 =  Heuristic::Sequential::cons_time(s);
	Route sch2 = mergeRoutes(rl2.routes);
	printBigRoute(sch2);
	SolutionF::labelRouteFitness(sch2, s);


/*	
	RouteList rl3 = par_cons_heu_dist(s);
	Route pch1 = mergeRoutes(rl3);
	printBigRoute(pch1);
	labelRouteFitness(pch1, s);

	RouteList rl4 = par_cons_heu_time(s);
	Route pch2 = mergeRoutes(rl4);
	printBigRoute(pch2);
	labelRouteFitness(pch2, s);
	

	// printBigRoute(theBestAnswer());
	// labelRouteFitness(theBestAnswer(), s);
	*/
	auto bestroutes = splitRoutes(theBestAnswer());
	 for(uint i = 0; i < bestroutes.size(); i++){
	 	if(test_feasibility(bestroutes[i], i, s)){
			std::cout << "Valid" << std::endl;
		} else {
			std::cout << "Invalid" << std::endl;
		}
		printRoute(bestroutes[i]);
		std::cout  << std::endl  << std::endl; 
	}

	return 0;
}