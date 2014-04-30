#ifndef ELLIPTICALARC_H
#define ELLIPTICALARC_H

#include "EdgeSegment.h"
#include"opencv2\imgproc\imgproc.hpp"
#include <set>

class EllipticalArc {
public:
	EllipticalArc();
	~EllipticalArc();

	/*true if this elliptical arc contains the specified edge segment */
	bool containsSegment(EdgeSegment*);

	void addSegment(EdgeSegment*);

	void joinArcs(EllipticalArc*);

	void drawToImage(cv::Mat*, cv::Vec3b);

	int const ID;
private:
	std::set<EdgeSegment*> segments;
	static int id_cnt;

};
#endif