#ifndef SEATS_HPP
#define SEATS_HPP

#include <vector>
#include <iostream>


struct Seats{
	std::vector<int> seats;

	void push_back(int s);
	void print();

	void accum(const Seats people);
	Seats plus(const Seats another);

	bool fit(const Seats people);
};

namespace SeatsF{
	Seats read(std::istream& src);
	Seats make(int i, int j, int k, int l);
}

#endif