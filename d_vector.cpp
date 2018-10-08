#include <algorithm>

#include "d_vector.hpp"


double DVector::sortSum() {
	std::vector<double> sum_cp(values.begin(), values.end());
	std::sort (sum_cp.begin(), sum_cp.end());
	double d = 0.0;
	for(auto k : values){
		d += k;
	}
	return d;
}

double DVector::value(int i) const {
	return values[i];
}

double DVector::update(int i, double nv) {
	return values[i] = nv;
}

void DVector::push_back(double v){
	values.push_back(v);
}