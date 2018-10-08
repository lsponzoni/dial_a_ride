#ifndef _POINT_HPP
#define _POINT_HPP

struct Point{
	double x;
	double y;
	void print();
};
namespace PointF{
	Point make(double x, double y);
	Point read();

	double distance(Point s, Point d);
}

#endif