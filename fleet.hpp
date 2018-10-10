#ifndef FLEET_HPP
#define FLEET_HPP

#include <vector>
#include "seats.hpp"
#include "index.hpp"

struct Fleet{
	int _size;
	std::vector<double> max_drive_time;
	std::vector<Seats> seats;

	void push_back(double drive_time, Seats v);

	Indexes labels();
	bool can_give_a_ride(Seats seats, int i);
	int size() const;
	void print() const;
};

namespace FleetF{
	Fleet read(int size,std::istream& src);
}

#endif