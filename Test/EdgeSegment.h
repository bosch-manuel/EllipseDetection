#ifndef EDGESEGMENT_H
#define EDGESEGMENT_H

#include "Point.h"
#include <list>

class EdgeSegment
{
public:
	EdgeSegment();
	~EdgeSegment();

	/*Adds a Point behind the currently last Point of the edge segment*/
	int push_backPoint(Point);

	/*Adds a Point at the beginning of the segment*/
	int push_frontPoint(Point);

	/*Approximates the Segment with connected straight lines*/
	int linearizeSegment();

	/*Draws the Segment into an image*/
	int drawSegment(Math image);

};

#endif
