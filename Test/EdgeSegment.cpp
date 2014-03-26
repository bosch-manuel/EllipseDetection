#include "EdgeSegment.h"
#include "Point.h"
#include <iostream>


EdgeSegment::EdgeSegment() {
	edgeList.clear();
}


EdgeSegment::~EdgeSegment() {
}

void EdgeSegment::push_backPoint(Point* p) {
	edgeList.push_back(p);
}

void EdgeSegment::push_frontPoint(Point* p) {
	edgeList.push_front(p);
}
