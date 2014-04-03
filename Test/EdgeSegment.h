#ifndef EDGESEGMENT_H
#define EDGESEGMENT_H

#include "Defines.h"
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

	size_t getLength();

	/*true if Segment has already been segmented into lines*/
	int isSegmented();

	/*Approximates the Segment with connected straight lines
	d_tol	max devation of a point to a line segment*/
	int lineSegmentation(int d_tol);

	/*Draws the Segment into image, Color is provied color (BGR))*/
	void drawToImage(cv::Mat *image,cv::Vec3b color);

private:
	std::list<Point*> edgeList; //edge pixels
	int segmented;
	int nLineSegs;

};

#endif
