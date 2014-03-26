#include "Point.h"


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
