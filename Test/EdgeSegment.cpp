#include "EdgeSegment.h"


EdgeSegment::EdgeSegment() {
	edgeList.clear();
}


EdgeSegment::~EdgeSegment()
{
}

void EdgeSegment::push_backPoint(Point* p) {
	edgeList.push_back(p);
}

void EdgeSegment::push_frontPoint(Point* p) {
	edgeList.push_front(p);
}

