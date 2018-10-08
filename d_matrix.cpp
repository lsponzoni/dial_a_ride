
#include <iostream>

#include "d_matrix.hpp"

double DMatrix::dist_points(int i, int j) const {
	if(i == j)
		return 0.0f; //I don't know why but returning 0.0f solves a numeric error
	if(i > j) 
		return matrix.at(j).value(i - j);
	return matrix.at(i).value(j - i);
}

void DMatrix::printUpper(){
	using namespace std;
	for(uint i = 0; i < matrix.size(); i++){
		for(uint j = i; j < matrix.size(); j++)
			cout << dist_points(i,j) << '\t';
		cout << endl << endl;	
	}
}

void DMatrix::printSquare(){
	using namespace std;
	for(uint i = 0; i < matrix.size(); i++){
		for(uint j = 0; j < matrix.size(); j++)
			cout << dist_points(i,j) << '\t';
		cout << endl << endl;	
	}
}

void DMatrix::push_back(DVector v){
	matrix.push_back(v);
}

DMatrix DMatrixF::memo(std::vector<Point> all){
	DMatrix dm;	
	for(uint i = 0; i < all.size(); i++){
		DVector vd;
		for(uint j = i; j < all.size(); j++){
			vd.push_back(PointF::distance(all[i], all[j]));
		}
		dm.push_back(vd);
	}
	return dm;
}

