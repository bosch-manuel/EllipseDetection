#ifndef EDGESEGMENT_H
#define EDGESEGMENT_H

#include "Point.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <list>

class EdgeSegment
{
public:
	EdgeSegment();
	~EdgeSegment();

	/*Adds a Point behind the currently last Point of the edge segment*/
	void push_backPoint(Point*);

	/*Adds a Point at the beginning of the segment*/
	void push_frontPoint(Point*);

	/*Approximates the Segment with connected straight lines*/
	int linearizeSegment();

	/*Draws the Segment into an image*/
	void drawSegment(cv::Mat image);

private:
	std::list<Point*> edgeList; //edge pixels

};

#endif
