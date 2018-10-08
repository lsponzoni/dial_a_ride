
#include <iostream>
#include "fleet.hpp"


void Fleet::push_back(double drive_time, Seats max_seats){
	max_drive_time.push_back(drive_time);
	seats.push_back(max_seats);
	_size++;
}

int Fleet::size() const{
	return _size;
}

void Fleet::print() const{
	using namespace std;
	for(uint v = 0; v < _size; v++){
		cout << max_drive_time[v] << ' ';
		(const_cast<Seats&>(seats[v])).print();
		cout << endl;
	}
		
}
Indexes Fleet::labels(){
	return range(0, size());
}

bool Fleet::can_give_a_ride(Seats riders, int i){
	return seats[i].fit(riders);
}

Fleet FleetF::read(int size){
	Fleet f = {size, std::vector<double>(size), std::vector<Seats>(size)};
	for(int i = 0; i < size; i++){
		std::cin >> f.max_drive_time[i];
		f.seats[i] = SeatsF::read();
	}
	return f;
}
