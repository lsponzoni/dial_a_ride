#include "darp.hpp"


Route theBestAnswer(){
	return { 0, 10, 5, 26, 21, 14, 30, 15, 31, 7, 16, 23, 32, 33,
			   0, 12, 6, 28, 22, 4, 11, 27, 20, 3, 19, 13, 29, 9, 8, 25, 24, 2, 18, 1, 17, 33 };
}

/*
 * Solution to Darp, based on Masmoudi et Al 2017
 * 
 * From Sophie Parrag
 * A(vertex,vehicle) arrival at vx by vh
 * B(vertex,vehicle) begin service at vx by vh
 * W(vertex,vehicle) waiting at vx by vh
 * Q(vertex,vehicle) load at vx by vh
 * L(i,[vertex,]vehicle) ride time of user i, when vehicle is at vertex 
 * 
 * Time window = [ei,li]
 * t(ij,k) = travel time for vehicle k == euclidian distance between i and j
 * c(ij,k) = travel cost
 * both can become t(ij) c(ij) assuming it does not depend on the vehicle
 * d(i) tempo de serviço no nodo 
 * A(j,k) = B(i,k) + d(i) + t(ij,k) 
 * B =  
 * 
 * Q(r,k, j) >= Q(r,k, i) + q(r,j) 	
 */
/*
typedef Solution Individual;
typedef std::vector<Individual> Population;

#include "randomized.hpp"
Population sample(int s, Population p){
	Population sample;
	sample.reserve(s);
	for(int i = 0; i < s; i++){
		int index = Random::get_1_to(p.size()) - 1;// 0 to size		
		sample.push_back(p[index]);
	}
	return sample;
}

Route fromIndividual(Individual i){
	return i.getBigRoute();
}

Individual makeIndividual(Route b_route){
	return Solution(splitRoute(b_route));
}

double fitness(Individual i, Instance ins){
	return gerequest_sizeouteFitness(fromIndividual(i), ins);
}

Individual tournament(Population p, Instance ins){
	int best = 0;
	double best_fitness = fitness(p[0], ins);
	for(int i = 0; i < p.size(); i++){
		if(best_fitness < fitness(p[i], ins)){
			best_fitness = fitness(p[i], ins);
			best = i;
		}
	}
	return p[best];
}

Individual xover_seq(Individual a, Individual b, int p){

}

Individual xover_assign(Individual a, Individual b, Instance s){

}

const int SAMPLE_SIZE = 4;
Route tournamentSelection(Population p, Instance i){
	return fromIndividual(tournament(sample(SAMPLE_SIZE, p), i));
}
*/