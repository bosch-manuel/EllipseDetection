#include "Point.h"
#include <math.h>

Point::Point(int y, int x) 
:y(y), x(x)
{}


Point::~Point(){}

int Point::getX() {
	return x;
}

int Point::getY(){
	return y;
}

double Point::operator*(Point p) {
	return  (x*p.x + y*p.y);
}

Point Point::operator+(Point p) {
	return Point(y + p.y, x + p.x);
}

Point Point::operator-(Point p){
	return Point(y - p.y, x - p.x);
}

double Point::norm() {
	return sqrt(double(x*x+y*y));
}
