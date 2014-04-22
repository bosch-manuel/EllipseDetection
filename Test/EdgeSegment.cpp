#include "EdgeSegment.h"
#include "Point.h"
#include "Defines.h"
#include <iostream>

using namespace std;

EdgeSegment::EdgeSegment()
: type(LSEG_EDGE)
, nLineSegs(-1){
	edgeList.clear();
}

EdgeSegment::EdgeSegment(int type)
: type(type)
, nLineSegs(-1){
	edgeList.clear();
}

EdgeSegment::~EdgeSegment() {
}

EdgeSegment::EdgeSegment( const EdgeSegment& other)
: type(other.type)
, nLineSegs(-1) {
	edgeList.clear();

	for (list<Point*>::const_iterator i = other.edgeList.begin(); i != other.edgeList.end(); i++)	{
		edgeList.push_back(new Point((*i)->getY(),(*i)->getX()));
	}
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

int EdgeSegment::getType() {
	return type;
}

/*Finds the point of max devation from the line between start and end
end will be set to this point
return max devation from line
Based on Peter Kovesi's matlab implementation: maxlinedev
Copyright (c) 2000-2006 Peter Kovesi
% School of Computer Science & Software Engineering
% The University of Western Australia
% http://www.csse.uwa.edu.au/
%
% Permission is hereby granted, free of charge, to any person obtaining a copy
% of this software and associated documentation files (the "Software"), to deal
% in the Software without restriction, subject to the following conditions:
%
% The above copyright notice and this permission notice shall be included in
% all copies or substantial portions of the Software.*/
double maxlinedev(list<Point*>::iterator *start, list<Point*>::iterator *end) {
	double D, y1my2, x2mx1, C,dev,maxDev;
	list<Point*>::iterator tmp;
	list<Point*>::iterator cur_max;
	//distance between end points
	//D = sqrt((x(1)-x(Npts))^2 + (y(1)-y(Npts))^2);
	maxDev = 0.0;
	D = sqrt(pow((**start)->getX() - (**end)->getX(), 2) + pow((**start)->getY() - (**end)->getY(), 2));

	y1my2 = (**start)->getY() - (**end)->getY();
	x2mx1 = (**end)->getX() - (**start)->getX();
	C = (**end)->getY() * (**start)->getX() - (**start)->getY() * (**end)->getX();

	//calulate distance from line for every point between the two end points
	for ( tmp = *start; tmp != *end; tmp++) {
		dev = abs((*tmp)->getX()*y1my2 + (*tmp)->getY()*x2mx1 + C) / D;
		if (dev>=maxDev) {
			cur_max = tmp;
			maxDev = dev;
		}
	}
#ifdef DEBUG_MAXLINEDEV
	cout << "MaxLineDev: " << (**start)->getY() << ", " << (**start)->getX() << " und " << (**end)->getY()<< ", "<< (**end)->getX();
#endif
	*end = cur_max;
#ifdef DEBUG_MAXLINEDEV
	cout << "-->   Max abw - "<< maxDev<<" - bei: " << (**end)->getY() << ", " << (**end)->getX() << endl;
#endif
	return maxDev;
}

int EdgeSegment::lineSegmentation(int d_tol) {
	nLineSegs = 0;
	double maxDev = 0;
	double dev = 0;
	int r_max, c_max;
	double D = 0;
	double y1my2 = 0;
	double x2mx1 = 0;
	double C = 0;
	int size = edgeList.size();
	list<Point*>::iterator last = edgeList.begin();
	list<Point*>::iterator lastElem = edgeList.begin();
	list<Point*>::iterator first = edgeList.begin();
	list<Point*>::iterator tmp;

	//set iterator last on last element
	for (size_t i = 0; i < size-1; i++){
		lastElem++;
	}
	last = lastElem;
	while (first != last) {
		maxDev=maxlinedev(&first, &last);
		//split line at point with maxDev if maxDev>tolDev
		while (maxDev>d_tol) {
			maxDev = maxlinedev(&first, &last);
		}
		//remove all Points between first and last (end points of the line Segment)
		if (first != last) {
			first = edgeList.erase(++first, last);
			last = edgeList.begin();
			//set iterator last on last element
			for (size_t i = 0; i < edgeList.size() - 1; i++){
				last++;
			}
		}
		nLineSegs++;
		//next line segment
		//first = last;
		
	}

	//Mark EdgeSegment as segmented
	type = LSEG_EDGE;
	return nLineSegs;
}

void EdgeSegment::drawToImage(cv::Mat *image,cv::Vec3b color) {
	cv::Scalar c(color[0], color[1], color[2]);
	if (type==EDGESEG) {
		//draw every contained Point to image
		for (std::list<Point*>::iterator it = edgeList.begin(); it != edgeList.end(); it++)	{
			image->at<cv::Vec3b>((*it)->getY(), (*it)->getX())[0] = color.val[0];
			image->at<cv::Vec3b>((*it)->getY(), (*it)->getX())[1] = color.val[1];
			image->at<cv::Vec3b>((*it)->getY(), (*it)->getX())[2] = color.val[2];
		}
	}else {
		//draw the lines between the end points
		std::list<Point*>::iterator a, a_1;
		cv::Point p1;
		cv::Point p2;
		a = edgeList.begin();
		a_1 = ++edgeList.begin();
		for (; a != edgeList.end() && a_1!=edgeList.end(); a++,a_1++)	{
			p1 = cv::Point((*a)->getX(), (*a)->getY());
			p2 = cv::Point((*a_1)->getX(), (*a_1)->getY());
			cv::line(*image,p1,p2,c,1,8);

		}
	}
}

int EdgeSegment::curveSegmentation(std::list<EdgeSegment*> *curveSegments) {
	Point *lastSplit=NULL; // Point where the last segmentation has taken place, all points left to lastSplit must not be considered for any further segmentation
	Point *L1 = NULL, *L2 = NULL, *R1 = NULL, *R2 = NULL, *P = NULL, *PL1 = NULL, *PR1 = NULL, *r0 = NULL
		, *r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL, *r5 = NULL, *r6 = NULL, *r7 = NULL, *r8 = NULL, *r9 = NULL;
	list<Point*>::const_iterator i, l, r;
	int lSteps,rSteps,lengthPL1,lengthPR1=0;
	int nCurvSegs = 0;
	int a1 = 0, a2 = 0, a3 = 0, a4 = 0, g1 = 0,g2=0,b1=0,b2=0,b3=0;
	EdgeSegment *cS = NULL;

	//segment must be segmented into lines
	if (type!=LSEG_EDGE) {
		return -1;
	}
	//go through the whole segment
	cS = new EdgeSegment(CURVESEG);
	for (i = edgeList.begin(); i !=edgeList.end(); i++)	{
		P = *i;
		cS->push_backPoint(P);//collect all visited points
		//check how many points exist left and right to the current point, to determine which condition should be tested
		for (lSteps=-1,l = i;*l!=lastSplit && l != edgeList.begin() && lSteps<2 ; l--,lSteps++)	{
			lSteps == 0 ? L1 = *l : L2 = *l;
		}

		for (rSteps = -1, r = i; r != edgeList.end() && *r != lastSplit && rSteps<2; r++, rSteps++) {
			rSteps == 0 ? R1 = *r : R2 = *r;
		}

		if (lSteps >= 1 && rSteps >= 1)	{ //length condition
			PL1 = &(*P - *L1);
			PR1 = &(*P - *R1);
			lengthPL1 = PL1->norm();
			lengthPR1 = PR1->norm();
			if (lengthPL1>LTH* lengthPR1 || lengthPR1 > LTH* lengthPL1) {
				//split the line at P
				curveSegments->push_back(cS);
				nCurvSegs++;
				lastSplit = P;// keep the last splitting point in mind
				cS = new EdgeSegment(CURVESEG); // next curve segment
				cS->push_backPoint(P);//collect all visited points

			}
		}
			//curvature condition left side
			else if(lSteps==2 && rSteps>=1) { 
				r0 = &(*L1 - *L2);
				r1 = &(*P - *L2);
				r2 = &(*R1 - *L2);
				a1 = acos((*r0 * *r1)/(r0->norm()* r1->norm()))*(180/PI);
				a2 = acos((*r0 * *r2) / (r0->norm()* r2->norm()))*(180 / PI);
				g1 = acos((*r1 * *r2) / (r1->norm()* r2->norm()))*(180 / PI);
				
				if (abs(a2 - a1) != g1 || (a2 - a1) < 0) {
					//split the line at P
					curveSegments->push_back(cS);
					nCurvSegs++;
					lastSplit = P;// keep the last splitting point in mind
					cS = new EdgeSegment(CURVESEG); // next curve segment
					cS->push_backPoint(P);//collect all visited points
				}

			}
			//curvature condition right side
			else if (rSteps == 2 && lSteps >= 1){
				r3 = &(*R1 - *R2);
				r4 = &(*L1 - *R2);
				r5 = &(*P - *R2);
				a3 = acos((*r3 * *r5) / (r3->norm()* r5->norm()))*(180 / PI);
				a4 = acos((*r3 * *r4) / (r3->norm()* r4->norm()))*(180 / PI);
				g2 = acos((*r4 * *r5) / (r4->norm()* r5->norm()))*(180 / PI);

				if (abs(a4 - a3) != g2 || (a4 - a3) < 0) {
					//split the line at P
					curveSegments->push_back(cS);
					nCurvSegs++;
					lastSplit = P;// keep the last splitting point in mind
					cS = new EdgeSegment(CURVESEG); // next curve segment
					cS->push_backPoint(P);//collect all visited points
				}

			}
			//angle condition
			else if (rSteps == 2 && lSteps == 2) {
				r7 = &(*P - *L1);
				r8 = &(*P - *R1);
				b3 = acos((*r0 * *r7) / (r0->norm()* r7->norm()))*(180 / PI);
				b1 = acos((*r3 * *r8) / (r3->norm()* r8->norm()))*(180 / PI);
				b2 = acos((*r7 * *r8) / (r7->norm()* r8->norm()))*(180 / PI);

				if (abs(b1 - b2) > TH || abs(b3 - b2) > TH) {
					//split the line at P
					curveSegments->push_back(cS);
					nCurvSegs++;
					lastSplit = P;// keep the last splitting point in mind
					cS = new EdgeSegment(CURVESEG); // next curve segment
					cS->push_backPoint(P);//collect all visited points
				}
			}
		

		}
	//add last curve segment to list
	curveSegments->push_back(cS);
	nCurvSegs++;

	return nCurvSegs;
}

