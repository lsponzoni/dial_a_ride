
#if !defined(D_VECTOR_HPP)
#define D_VECTOR_HPP

#include <vector>
struct DVector{
	std::vector<double> values;
	
	void push_back(double d);
	double value(int i) const ;
	double update(int i, double nv);
	double sortSum() ;
};



#endif // D_VECTOR_HPP
