
#include <iostream>
#include <cassert>
#include "requests.hpp"

int Requests::size() const {
	return point_total;
}

Indexes Requests::pickup_labels(){
	return range(1, request_total);
}

Indexes Requests::vertex_labels(){
	return range(0, point_total);
}

bool Requests::isDepot(int label) const { 
	return label == src() || label >= dst();
}

bool Requests::isPickup(int label) const { 
	return 0 < label && label <= request_total;
}

bool Requests::isDelivery(int label) const { 
	return request_total < label && label < dst();
}

Point Requests::getPoint(int label) const{
	return _points[label];
}

int Requests::pickup_to(int label) const{
	if(label == dst())
		return src();
	if(isDelivery(label))
		return label - request_total;
	return label;
}

int Requests::delivery_to(int label) const{
	if(label == src())
		return dst();
	if(isPickup(label))
		return label + request_total;
	return label;
}

void Requests::print() const{
	using namespace std;
	for(int i = 0; i < size(); i++){
		cout << i << '\t';
		getPoint(i).print();
		TimeWindowF::print(load_time(i), ride_time(i));
		seats(i).print();
		TimeWindowF::print(early(i), late(i));
		cout << endl;
	}
	cout << endl;
}

int Requests::src() const{
	return 0;
}

int Requests::dst() const{
	return point_total - 1;
}

double Requests::early(int label) const{
	return _early[label];
}
double Requests::late(int label) const{
	return _late[label];
}

double Requests::load_time(int label) const{
	return _load_time[label];
}

double Requests::ride_time(int label) const {
	int l = pickup_to(label);
	return _ride_time[l];
}

double Requests::max_wait(int label) const {
	return 300;//unnecessarily big number
}
Seats Requests::seats(int label) const{
	return _seats[label];
}

Requests RequestsF::read(int size){
	int _all_size = (2 * size) + 2 ;
	std::vector<Point> points(_all_size);
	std::vector<double> ride_time(_all_size);
	std::vector<double> load_time(_all_size);
	std::vector<double> early(_all_size);
	std::vector<double> late(_all_size);
	std::vector<Seats> seats(_all_size);

	int label;
	for(int i = 0; i < _all_size; i++){
		std::cin >> label;
		assert(label == i);
		std::cin >> points[label].x >> points[label].y;
		std::cin >> load_time[label];
		std::cin >> ride_time[label];
		seats[i] = SeatsF::read();
		std::cin >> early[label] >> late[label];
		
	}
	return RequestsF::make(size, _all_size, points, early, late, load_time , ride_time, seats);
}

Requests RequestsF::make(int requests, int point_total, 
	std::vector<Point> points,
	std::vector<double> early,
	std::vector<double> late,
	std::vector<double> load_time,
	std::vector<double> ride_time,
	std::vector<Seats> seats)
{
	return {
		requests, point_total ,
		points, early, late, load_time, ride_time, seats
	};
}