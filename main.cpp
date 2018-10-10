#include "darp.hpp"
#include "feasibility.hpp"
#include <fstream>

void parraugh_solution_feas_check(Instance s){
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
	
}

void tangTest(Instance s){
	Route best = {0,1,2,3,4,5};

	if(test_feasibility(best, s.fleet.labels()[0], s))
		printRoute(best);
}
int main(int argc, char* argv[]){
	using namespace std;
	if(argc < 1){
		cerr << "Not enough arguments";
		exit(1);
	}
	ifstream infile;
	infile.open(argv[1]);
	if (!infile) {
    	cerr << "Unable to open file "<< argv[1];
		exit(1);
	}
	Instance s = InstanceF::read(infile);
	infile.close();

	parraugh_solution_feas_check(s);
	//Solution rl1 = Heuristic::Sequential::cons_distance(s);
	//Route sch1 = mergeRoutes(rl1.routes);
	// printBigRoute(sch1);
	// SolutionF::labelRouteFitness(sch1, s);

	// Solution rl2 =  Heuristic::Sequential::cons_time(s);
	// Route sch2 = mergeRoutes(rl2.routes);
	//printBigRoute(sch2);
	//SolutionF::labelRouteFitness(sch2, s);


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

	// testA(s);
	//printScheduleConstraints(bestroutes[0], s);
	//printScheduleConstraints(bestroutes[1], s);
	return 0;
}