#ifndef EDGESEGMENT_H
#define EDGESEGMENT_H

#include "Defines.h"
#include "Point.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <list>

#define LSEG_EDGE 0 //line segmented edge segment
#define EDGESEG  1 // pure edge segment
#define CURVESEG 2 // curve segment
class EdgeSegment
{
public:
	EdgeSegment();
	EdgeSegment(int type);
	~EdgeSegment();
	EdgeSegment(const EdgeSegment& other);
	/*Adds a Point behind the currently last Point of the edge segment*/
	void push_backPoint(Point*);

	/*Adds a Point at the beginning of the segment*/
	void push_frontPoint(Point*);

	size_t getLength();

	/*true if Segment has already been segmented into lines*/
	int getType();

	/*Approximates the Segment with connected straight lines
	d_tol	max devation of a point to a line segment*/
	int lineSegmentation(int d_tol);

	/*Draws the Segment into image, Color is provied color (BGR))*/
	void drawToImage(cv::Mat *image,cv::Vec3b color);

	/*Curve segmentation based on the approach in "A real-time ellipse detection based on edge grouping" by Thanh Minh Nguyen, Siddhant Ahuja and Q. M. Jonathan Wu
	to get a result, the edge segment must have been segmented into lines!
	param:
		curveSegments	list for curve segments generated from this edge segment
	return
		num of curve segments; -1 if edge segment was not segmented*/
	int curveSegmentation(std::list<EdgeSegment*> *curveSegments,std::fstream *csf);

	
private:
	std::list<Point*> edgeList; //edge pixels
	int nLineSegs;
	int type;

};

#endif
