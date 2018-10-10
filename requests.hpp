#ifndef REQUESTS_HPP
#define REQUESTS_HPP

#include "index.hpp"
#include "time_window.hpp"
#include "point.hpp"
#include "seats.hpp"
#include <vector>

struct Requests{
	int request_total;
	int point_total;
	std::vector<Point> _points;
	std::vector<double> _early, _late,
		 _load_time, _ride_time;
	std::vector<Seats> _seats;

	Indexes pickup_labels();

	double early(int label) const;
	double late(int label)const;
	double load_time(int label)const;
	double ride_time(int label)const;
	Seats seats(int label)const;
	
	double max_wait(int label)const;
	Indexes vertex_labels();

	int src() const;
	int dst() const;

	int size() const;
	bool isDepot(int label) const;
	bool isPickup(int label) const;
	bool isDelivery(int label) const;
	int pickup_to(int label) const;
	int delivery_to(int label) const;
	void print() const;

	Point getPoint(int label) const;
};

namespace RequestsF{
	Requests make(
		int requests, int point_total,
		std::vector<Point> points,
		std::vector<double> early,
		std::vector<double> late,
		std::vector<double> load_time,
		std::vector<double> ride_time,
		std::vector<Seats> seats
	);

	Requests read(int size);
}

#endif