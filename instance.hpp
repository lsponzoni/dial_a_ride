#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "requests.hpp"
#include "fleet.hpp"
#include "d_matrix.hpp"
#include <iostream>
struct Instance{
	Fleet fleet;
	Requests request_table;
	DMatrix dm;
	

	Point getPoint(int label) const;
	int src_to(int label) const;
	int dst_to(int label) const;

	int vehiclesTotal() const { return fleet.size(); }
	int requestsTotal() const { return request_table.request_total; }
	

	void print() const;
};

namespace InstanceF{
	Instance read(std::istream& src);
	Instance make(Fleet f, Requests r);
}




#endif