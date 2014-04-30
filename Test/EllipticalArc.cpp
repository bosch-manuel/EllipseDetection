#include "EllipticalArc.h"

EllipticalArc::EllipticalArc()
:ID(++id_cnt){}

EllipticalArc::~EllipticalArc() {
	segments.clear();
}

bool EllipticalArc::containsSegment(EdgeSegment* cS) {
	return segments.find(cS) != segments.end() ? true : false;
}

void EllipticalArc::addSegment(EdgeSegment* cS) {
	segments.insert(cS);
}

void EllipticalArc::joinArcs(EllipticalArc* eA) {
	for (std::set<EdgeSegment*>::iterator i = eA->segments.begin(); i != eA->segments.end(); i++) {
		segments.insert(*i);
	}
}

void EllipticalArc::drawToImage(cv::Mat* image, cv::Vec3b color) {
	for (std::set<EdgeSegment*>::const_iterator i = segments.begin(); i !=segments.end(); i++)	{
		(*i)->drawToImage(image,color);
	}
}

int EllipticalArc::id_cnt = 0;