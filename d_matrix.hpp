#if !defined(D_MATRIX_HPP)
#define D_MATRIX_HPP

#include <vector>

#include "d_vector.hpp"

struct DMatrix{
	std::vector<DVector> matrix;

	void push_back(DVector v);
	double dist_points(int i, int j) const ;

	//Results are Top Right, and shifted so DM diag = (i,0) instead of (i,i) 
	void printUpper();
	void printSquare();
};

#include "point.hpp"

namespace DMatrixF{
	DMatrix memo(std::vector<Point> all);
}
#endif // D_MATRIX_HPP
