#include "point.hpp"

#include <cmath>
#include <iostream>

void Point::print(){
	std::cout << x << '\t' << y;
}

Point PointF::make(double x, double y){
	Point p = {x, y};
	return p;
}

Point PointF::read(){
	double x, y;
	std::cin >> x >> y;
	return make(x,y); 
}

double PointF::distance(Point s, Point d){
	return hypot(d.x - s.x, d.y - s.y);
}

