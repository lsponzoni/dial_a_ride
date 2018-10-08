#include "time_window.hpp"
#include <iostream>


void TimeWindowF::print(double begin, double end){
	std::cout << '\t' << begin << ' ' << end << '\t';
}

double TimeWindowF::tw_size(double begin, double end){
	return end - begin;
}

double TimeWindowF::tw_midpoint(double begin, double end){
	return (begin + end)/2;
}
