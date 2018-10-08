#include <random>
#include "randomized.hpp"

int Random::get(int min, int max){
	std::default_random_engine gen;
	std::uniform_int_distribution<int> ui_distribution(min, max);
	return ui_distribution(gen);
}

int Random::get_less_than(int max){
	return get(0, max);
}

int Random::get_1_to(int max){
	return get(1,max);
}
