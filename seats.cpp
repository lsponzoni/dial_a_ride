
#include <iostream>

#include "seats.hpp"

/* Example of DARP Fitting matrix(people x places)
 * 	 a b c d 
 *a  * * *
 *b	   * *
 *c      * 
 *d        *
 */ 
const int capacities = 4;
const int strict_capacities = capacities - 1;

void Seats::print(){
	using namespace std;
	for(int s: seats)
		cout << s << ' ';
}

void Seats::push_back(int s){
	seats.push_back(s);
}

void Seats::accum(const Seats people){
	for(int i = 0; i < capacities; i++)
		seats[i] = (seats[i] + people.seats[i]);
}

Seats Seats::plus(Seats another){
	Seats r;
	for(int i = 0; i < capacities; i++)
		r.push_back(seats[i] + another.seats[i]);
	return r;
}

bool Seats::fit(const Seats people){
	int toAdd = 0;
	
	for(int i = 0; i < strict_capacities; i++){
		if(seats[i] < people.seats[i] + toAdd){
			toAdd = toAdd + people.seats[i] - seats[i];
		} else {
			toAdd = 0;
		}
	}	

	for(int i = strict_capacities; i < capacities; i++)
		if(seats[i] < people.seats[i])
			return false;
	return toAdd == 0;
}

Seats SeatsF::make(int i, int j, int k, int l){
	Seats s = {{i,j,k,l}};
	return s;
}

Seats SeatsF::read(){
	using namespace std;
	Seats c;
	for(int j = 0; j < capacities; j++){
		int cap;
		cin >> cap;
		c.push_back(cap);
	}
	return c;
}