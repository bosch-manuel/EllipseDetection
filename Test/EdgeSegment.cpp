#include "EdgeSegment.h"
#include "Point.h"
#include <iostream>


EdgeSegment::EdgeSegment() {
	edgeList.clear();
	isSegmented = false;
}


EdgeSegment::~EdgeSegment() {
}

void EdgeSegment::push_backPoint(Point* p) {
	edgeList.push_back(p);
}

void EdgeSegment::push_frontPoint(Point* p) {
	edgeList.push_front(p);
}

size_t EdgeSegment::getLength() {
	return edgeList.size();
}

bool EdgeSegment::isSegmented() {
	return isSegmented;
}

void EdgeSegment::drawToImage(cv::Mat image,cv::Vec3b color) {
	std::list<Point*>::iterator it;
	//draw every contained Point to image
	for ( it = edgeList.begin(); it !=edgeList.end(); it++)	{
		image.at<cv::Vec3b>((*it)->getY(), (*it)->getX())[0] = color.val[0];
		image.at<cv::Vec3b>((*it)->getY(), (*it)->getX())[1] = color.val[1];
		image.at<cv::Vec3b>((*it)->getY(), (*it)->getX())[2] = color.val[2];
	}
}

