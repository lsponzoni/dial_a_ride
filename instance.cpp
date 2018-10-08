#include "instance.hpp"

Point Instance::getPoint(int label) const{
	return request_table.getPoint(label);
}

int Instance::src_to(int label) const{
	return request_table.pickup_to(label);
}

int Instance::dst_to(int label) const{
	return request_table.delivery_to(label);
}

void Instance::print() const{
	using namespace std;
	cout << vehiclesTotal() << ' ' << requestsTotal() << endl;
	fleet.print();
	request_table.print();
}



Instance InstanceF::make(Fleet f, Requests r) 
{
	DMatrix dMatrix = DMatrixF::memo(r._points);
	Instance inst = {f, r, dMatrix};
	return inst;
}

Instance InstanceF::read(){
	using namespace std;
	int vehicles, requests;
	cin >> vehicles >> requests;
	Fleet f = FleetF::read(vehicles);
	Requests r = RequestsF::read(requests);
	return make(f, r);
}
