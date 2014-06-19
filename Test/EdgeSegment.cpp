#include "EdgeSegment.h"
#include "Point.h"
#include "Defines.h"
#include "PreProcessing.hpp"
#include "Conic.h"
#include <climits>
#include "\eigen-eigen-6b38706d90a9\Eigen\Eigenvalues"
#include <iostream>

using namespace std;

int EdgeSegment::id_cnt = 0;

EdgeSegment::EdgeSegment()
: type(LSEG_EDGE)
, nLineSegs(-1)
, ID(++id_cnt){
	edgeList.clear();
}

EdgeSegment::EdgeSegment(int type)
: type(type)
, nLineSegs(-1)
, ID(++id_cnt){
	edgeList.clear();
}

EdgeSegment::~EdgeSegment() {
	edgeList.clear();
}

EdgeSegment::EdgeSegment(const EdgeSegment& other)
: type(other.type)
, nLineSegs(-1)
, ID(++id_cnt){
	edgeList.clear();

	for (list<Point*>::const_iterator i = other.edgeList.begin(); i != other.edgeList.end(); i++)	{
		edgeList.push_back(new Point((*i)->getY(), (*i)->getX()));
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

//bool  EdgeSegment::operator<(const EdgeSegment& other) const {
//	return ID < other.ID;
//}
//
//bool EdgeSegment::operator>(const EdgeSegment& other) const {
//	return ID > other.ID;
//}

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
	double D, y1my2, x2mx1, C, dev, maxDev;
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
	for (tmp = *start; tmp != *end; tmp++) {
		dev = abs((*tmp)->getX()*y1my2 + (*tmp)->getY()*x2mx1 + C) / D;
		if (dev >= maxDev) {
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

list<Point*>::const_iterator EdgeSegment::cbegin() {
	return edgeList.cbegin();
}

list<Point*>::const_iterator EdgeSegment::cend() {
	return edgeList.cend();
}

list<Point*>::const_reverse_iterator EdgeSegment::crbegin() {
	return edgeList.crbegin();
}

list<Point*>::const_reverse_iterator EdgeSegment::crend() {
	return edgeList.crend();
}

Point* EdgeSegment::getFirstPoint(){
	return edgeList.front();
}

Point* EdgeSegment::getLastPoint(){
	return edgeList.back();
}
//
Point* EdgeSegment::getNextToLastPoint() {
	list<Point*>::reverse_iterator i = edgeList.rbegin();
	if (i != edgeList.rend()) {
		i++;
	}
	return *i;
}

//Point* EdgeSegment::getNextToNextToLastPoint() {
//	list<Point*>::reverse_iterator i = edgeList.rbegin();
//	if (i != edgeList.rend()) {
//		i++;
//		i++;
//	}
//	return *i;
//}

Point* EdgeSegment::getSecondPoint() {
	list<Point*>::iterator i = edgeList.begin();
	if (i != edgeList.end()) {
		i++;
	}
	return *i;
}

//Point* EdgeSegment::getThirdPoint() {
//	list<Point*>::iterator i = edgeList.begin();
//	if (i != edgeList.end()) {
//		i++;
//		i++;
//	}
//	return *i;
//}

//void EdgeSegment::addSegment(EdgeSegment *seg, int order) {
//	switch (order) {
//	case END_AT_END:
//		for (std::list<Point*>::const_reverse_iterator i = seg->edgeList.rbegin(); i != seg->edgeList.rend(); i++)	{
//			edgeList.push_back(*i);
//		}
//		break;
//	case END_AT_BEGIN:
//		for (std::list<Point*>::const_reverse_iterator i = seg->edgeList.rbegin(); i != seg->edgeList.rend(); i++)	{
//			edgeList.push_front(*i);
//		}
//			break;
//	case BEGIN_AT_END:
//		for (std::list<Point*>::const_iterator i = seg->edgeList.begin(); i != seg->edgeList.end(); i++)	{
//			edgeList.push_back(*i);
//		}
//		break;
//	case BEGIN_AT_BEGIN:
//		for (std::list<Point*>::const_iterator i = seg->edgeList.begin(); i != seg->edgeList.end(); i++)	{
//			edgeList.push_front(*i);
//		}
//		break;
//	default:
//		break;
//	}
//}

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
	int help = ID;
	list<Point*>::iterator last = edgeList.begin();
	list<Point*>::iterator lastElem = edgeList.begin();
	list<Point*>::iterator first = edgeList.begin();
	list<Point*>::iterator tmp;

	//set iterator last on last element
	for (size_t i = 0; i < size - 1; i++){
		lastElem++;
	}
	last = lastElem;
	while (first != last) {
		maxDev = maxlinedev(&first, &last);
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

void EdgeSegment::drawToImage(cv::Mat *image, cv::Vec3b color) {
	cv::Scalar c(color[0], color[1], color[2]);
	if (type == EDGESEG) {
		//draw every contained Point to image
		for (std::list<Point*>::iterator it = edgeList.begin(); it != edgeList.end(); it++)	{
			image->at<cv::Vec3b>((*it)->getY(), (*it)->getX())[0] = color.val[0];
			image->at<cv::Vec3b>((*it)->getY(), (*it)->getX())[1] = color.val[1];
			image->at<cv::Vec3b>((*it)->getY(), (*it)->getX())[2] = color.val[2];
		}
	}
	else {
#ifdef DRAW_POINTS
		for (std::list<Point*>::iterator it = edgeList.begin(); it != edgeList.end(); it++)	{
			image->at<cv::Vec3b>((*it)->getY(), (*it)->getX())[0] = color.val[0];
			image->at<cv::Vec3b>((*it)->getY(), (*it)->getX())[1] = color.val[1];
			image->at<cv::Vec3b>((*it)->getY(), (*it)->getX())[2] = color.val[2];
		}
#else
		//draw the lines between the end points
		std::list<Point*>::iterator a, a_1;
		cv::Point p1;
		cv::Point p2;
		a = edgeList.begin();
		a_1 = ++edgeList.begin();
		for (; a != edgeList.end() && a_1 != edgeList.end(); a++, a_1++)	{
			p1 = cv::Point((*a)->getX(), (*a)->getY());
			p2 = cv::Point((*a_1)->getX(), (*a_1)->getY());
			cv::line(*image, p1, p2, c, 1, 8);

		}
#endif
	}
}

///*Test whether the segment should be splitted at P inregard to the length condition
//param
//	L1		point left to P
//	R1		point right to P
//return
//	true	segment must be splitted at P
//	false	else*/
//bool lengthCond(Point* L1, Point *P, Point *R1) {
//	Point *PL1, *PR1;
//	double lengthPL1, lengthPR1;
//	PL1 = &(*P - *L1);
//	PR1 = &(*P - *R1);
//	lengthPL1 = PL1->norm();
//	lengthPR1 = PR1->norm();
//	if (lengthPL1 > LTH* lengthPR1 || lengthPR1 > LTH* lengthPL1) {
//		return true;
//	}
//	else {
//		return false;
//	}
//}
//
///*Test whether the segment should be splitted at P in regard to the curvature condition
//param
//	L2		point before L1
//	L1		point before P
//	P		point before R1
//	R1		point after P
//return
//	true	segment should be splitted at P
//	fase	else
//	*/
//bool curvatureCond(Point *L2, Point *L1, Point *P, Point *R1) {
//	Point *r0, *r1, *r2;
//	double a1, a2, g1;
//	int d;
//
//	r0 = &(*L1 - *L2);
//	r1 = &(*P - *L2);
//	r2 = &(*R1 - *L2);
//	a1 = acos((*r0 * *r1) / (r0->norm()* r1->norm()))*(180 / PI);
//	a2 = acos((*r0 * *r2) / (r0->norm()* r2->norm()))*(180 / PI);
//	g1 = acos((*r1 * *r2) / (r1->norm()* r2->norm()))*(180 / PI);
//
//	d = (abs(a2 - a1) - g1) + .5;
//	if (d != 0 || (a2 - a1) < 0) {
//		return true;
//	}
//	else {
//		return false;
//	}
//}
//
///*Test whetherthe segment should be splitted at P in regard to the curvature condition
//param
//	L2		point before L1
//	L1		point before P
//	P		point before R1
//	R1		point after P
//	*/
//bool angleCond(Point *L2, Point *L1, Point *P, Point *R1, Point *R2) {
//
//}

bool EdgeSegment::evaluateCurvature(std::fstream *csf) {

	Point *l1=NULL, *l2=NULL, *P1, *P2, *P3;
	double b1, b2, deg,kp;
	b1 = -1;
#ifdef DEBUG_EVAL_CURVE
	*csf << "Winkelverlauf von Segment " << ID <<"Anz. Punkte: "<<this->getLength()<< endl;
	for (std::list<Point*>::const_iterator i = edgeList.cbegin(); i !=edgeList.cend(); i++)
	{
		*csf << "(" << (*i)->getX() << "," << (*i)->getY() << ")";
	} 
	*csf << endl;
#endif
	if (/*type == CURVESEG &&*/ edgeList.size() > 2) {
		std::list<Point*>::const_iterator i = edgeList.begin();
		std::list<Point*>::const_iterator j = i;
		j++;
		std::list<Point*>::const_iterator k = j;
		k++;
		for (; k != edgeList.end(); k++)	{
			P1 = (*i);
			P2 = (*j);
			P3 = (*k);
			l1 = &(*P1 - *P2);
			l2 = &(*P3 - *P2);

			b2 = acos((*l1 * *l2) / (l1->norm()* l2->norm()));
			kp = l1->getX()*l2->getY() - l1->getY()*l2->getX();
			*csf <<"kp(" << kp << ")";
			if (b1 != -1){
				//calc dif
#ifdef DEBUG_EVAL_CURVE
				deg = b2*(180 / PI);
				//kp = l1->getX()*l2->getY() - l1->getY()*l2->getX();
				*csf << deg /*<<"kp("<<kp<<")"*/<< ", ";
#endif
				if (abs(b1 - b2) > TH /*|| b1<B_MIN || b1>B_MAX*/) {
#ifdef DEBUG_EVAL_CURVE
					*csf << endl << "####### Trennen (" << P2->getX() << "," << P2->getY() << ")>>" << abs(b1 - b2)*(180 / PI) << endl;
#endif
					//return false;
				}
			}

			i = j;
			j = k;
			b1 = b2;
		}
#ifdef DEBUG_EVAL_CURVE
		deg = b2*(180 / PI);
		//kp = l1->getX()*l2->getY() - l1->getY()*l2->getX();
		*csf << deg /*<< "kp(" << kp << ")"*/ <<endl;
#endif

	}
	else {
#ifdef DEBUG_EVAL_CURVE
		*csf << endl << "#######" << endl;
#endif
		return false;
	}

#ifdef DEBUG_EVAL_CURVE
	*csf << endl << "#######" << endl;
#endif
	return true;
}

int EdgeSegment::curveSegmentation(std::list<EdgeSegment*> *curveSegments, std::fstream *csf) {
	Point *lastSplit = NULL; // Point where the last segmentation has taken place, all points left to lastSplit must not be considered for any further segmentation
	Point *L1 = NULL, *L2 = NULL, *R1 = NULL, *R2 = NULL, *P = NULL, *PL1 = NULL, *PR1 = NULL, *r0 = NULL
		, *r1 = NULL, *r2 = NULL, *r3 = NULL, *r4 = NULL, *r5 = NULL, *r6 = NULL, *r7 = NULL, *r8 = NULL, *r9 = NULL;
	list<Point*>::const_iterator i, l, r;
	int d, lSteps, rSteps, lengthPL1, lengthPR1 = 0;
	int nCurvSegs = 0;
	double a1 = 0, a2 = 0, a3 = 0, a4 = 0, g1 = 0, g2 = 0, b1 = 0, b2 = 0, b3 = 0;
	EdgeSegment *cS = NULL;

	//segment must be segmented into lines
	if (type != LSEG_EDGE) {
		return -1;
	}
	//go through the whole segment
	cS = new EdgeSegment(CURVESEG);
#ifdef DEBUB_CURVE_SEG
	*csf << "Segmentierung von Seg " << ID << endl;
#endif
	for (i = edgeList.begin(); i != edgeList.end(); i++)	{
		P = *i;
		cS->push_backPoint(P);//collect all visited points
		//check how many points exist left and right to the current point, to determine which condition should be tested
		for (lSteps = -1, l = i;/* *l != lastSplit &&*/ l != edgeList.begin() && lSteps < 2; l--, lSteps++)	{
			lSteps == 0 ? L1 = *l : L2 = *l;
		}

		for (rSteps = -1, r = i; r != edgeList.end() &&/* *r != lastSplit &&*/ rSteps < 2; r++, rSteps++) {
			rSteps == 0 ? R1 = *r : R2 = *r;
		}

		if (lSteps >= 1 && rSteps >= 1)	{ //length condition
			/*PL1 = &(*P - *L1);
			PR1 = &(*P - *R1);
			lengthPL1 = PL1->norm();
			lengthPR1 = PR1->norm();*/
			//if (lengthPL1>LTH* lengthPR1 || lengthPR1 > LTH* lengthPL1) {
			if (lengthCond(L1, P, R1)) {
#ifdef DEBUB_CURVE_SEG
				*csf << "lenght cond at: " << P->getX() << ", " << P->getY() << endl;
#endif
				//split the line at P
				lastSplit = P;// keep the last splitting point in mind
				if (cS->getLength() > NP) {
					curveSegments->push_back(cS);
					nCurvSegs++;
				}
				cS = new EdgeSegment(CURVESEG); // next curve segment
				cS->push_backPoint(P);//collect all visited points
				continue;
			}
		}
		//curvature condition left side
		if (lSteps == 2 && rSteps >= 1) {
			/*r0 = &(*L1 - *L2);
			r1 = &(*P - *L2);
			r2 = &(*R1 - *L2);
			a1 = acos((*r0 * *r1) / (r0->norm()* r1->norm()))*(180 / PI);
			a2 = acos((*r0 * *r2) / (r0->norm()* r2->norm()))*(180 / PI);
			g1 = acos((*r1 * *r2) / (r1->norm()* r2->norm()))*(180 / PI);

			d = (abs(a2 - a1)-g1) + .5;*/
			//if (d != 0 || (a2 - a1) < 0) {
			if (curvatureCond(L2, L1, P, R1)) {
#ifdef DEBUB_CURVE_SEG
				*csf << "curve cond left at: " << P->getX() << ", " << P->getY() << endl;
#endif
				//split the line at P
				if (cS->getLength() > NP) {
					curveSegments->push_back(cS);
					nCurvSegs++;
				}
				lastSplit = P;// keep the last splitting point in mind
				cS = new EdgeSegment(CURVESEG); // next curve segment
				cS->push_backPoint(P);//collect all visited points
				continue;
			}

		}
		//curvature condition right side
		if (rSteps == 2 && lSteps >= 1){
			/*r3 = &(*R1 - *R2);
			r4 = &(*L1 - *R2);
			r5 = &(*P - *R2);
			a3 = acos((*r3 * *r5) / (r3->norm()* r5->norm()))*(180 / PI);
			a4 = acos((*r3 * *r4) / (r3->norm()* r4->norm()))*(180 / PI);
			g2 = acos((*r4 * *r5) / (r4->norm()* r5->norm()))*(180 / PI);

			d = (abs(a4 - a3) - g2) + .5;*/
			//if (d !=0 || (a4 - a3) < 0) {
			if (curvatureCond(R2, R1, P, L1)) {
#ifdef DEBUB_CURVE_SEG
				*csf << "curve cond right at: " << P->getX() << ", " << P->getY() << endl;
#endif
				//split the line at P
				if (cS->getLength() > NP) {
					curveSegments->push_back(cS);
					nCurvSegs++;
				}
				lastSplit = P;// keep the last splitting point in mind
				cS = new EdgeSegment(CURVESEG); // next curve segment
				cS->push_backPoint(P);//collect all visited points
				continue;
			}

		}
		//angle condition
		if (rSteps == 2 && lSteps == 2) {
			/*r6 = &(*L2 - *L1);
			r7 = &(*P - *L1);
			r8 = &(*P - *R1);
			r9 = &(*R2 - *R1);

			b3 = acos((*r6 * *r7) / (r6->norm()* r7->norm()))*(180 / PI);
			b1 = acos((*r8 * *r9) / (r8->norm()* r9->norm()))*(180 / PI);
			b2 = acos((*r7 * *r8) / (r7->norm()* r8->norm()))*(180 / PI);

			if (abs(b1 - b2) > TH && abs(b3 - b2) > TH) {*/
			if (angleCond(L2, L1, P, R1, R2)) {
#ifdef DEBUB_CURVE_SEG
				*csf << "angle cond at: " << P->getX() << ", " << P->getY() << endl;
#endif
				//split the line at P
				if (cS->getLength() > NP) {
					curveSegments->push_back(cS);
					nCurvSegs++;
				}
				lastSplit = P;// keep the last splitting point in mind
				cS = new EdgeSegment(CURVESEG); // next curve segment
				cS->push_backPoint(P);//collect all visited points
				continue;
			}
		}


	}
	//add last curve segment to list
	if (cS->getLength() > NP) {
		curveSegments->push_back(cS);
		nCurvSegs++;
	}

	return nCurvSegs;
}

int EdgeSegment::curveSegmentationImproved(std::list<EdgeSegment*> *curveSegments, std::fstream *csf) {
	Point *P1, *P2=NULL, *P3=NULL, *l1, *l2;
	int nCurvSegs = 0;
	double a_pre = -1,a=-1,deg=-1,kp=0,kp_pre=0;
	EdgeSegment *cS = NULL;

	//segment must be segmented into lines and more than 2 points
	if (type != LSEG_EDGE && edgeList.size()>2) {
		return -1;
	}
	//go through the whole segment
	cS = new EdgeSegment(CURVESEG);
#ifdef DEBUB_CURVE_SEG
	*csf << "Segmentierung von Seg " << ID << endl;
#endif
	std::list<Point*>::const_iterator i = edgeList.begin();
	std::list<Point*>::const_iterator j = i;
	j++;
	std::list<Point*>::const_iterator k = j;
	k++;
	for (; k != edgeList.end(); i++,j++,k++){
		P1 = (*i);
		P2 = (*j);
		P3 = (*k);
		l1 = &(*P1 - *P2);
		l2 = &(*P3 - *P2);
		cS->push_backPoint(P1);//collect all visited points

		a = acos((*l1 * *l2) / (l1->norm()* l2->norm()));
		kp = l1->getX()*l2->getY() - l1->getY()*l2->getX();
#ifdef DEBUB_CURVE_SEG
		
		*csf << "kp(" << kp << ")";
#endif
		if (a_pre != -1){
			//calc dif
#ifdef DEBUB_CURVE_SEG
			deg = a_pre*(180 / PI);
			*csf << deg << ", ";
#endif
			if (abs(a_pre - a) > TH ||(kp*kp_pre)<=0 /*|| b1<B_MIN || b1>B_MAX*/) {
#ifdef DEBUB_CURVE_SEG
				*csf << endl << "####### Trennen (" << P2->getX() << "," << P2->getY() << ")>>" << abs(a - a_pre)*(180 / PI) << endl;
#endif
				//split the segment at P2 and add the curve segmente if it contains more than NP points
				if (cS->getLength() > NP) {
					curveSegments->push_back(cS);
					nCurvSegs++;
				}
				a_pre = -1;
				cS = new EdgeSegment(CURVESEG);
			}
		}

		/*i = j;
		j = k;*/
		a_pre = a;
		kp_pre = kp;
	
		
	}
	//add two last points to the current curve segments
	cS->push_backPoint(P2);
	cS->push_backPoint(P3);
	//add last curve segment to list
	if (cS->getLength() > NP) {
		curveSegments->push_back(cS);
		nCurvSegs++;
	}

	return nCurvSegs;
}

/*
Based on optimized MAtlab code from "Direct Least Squares Fitting for Ellipses"
http://research.microsoft.com/en-us/um/people/awf/ellipse/fitellipse.html
*/
Ellipse* EdgeSegment::calcEllipse() {
	if (edgeList.size() < 6) {
		return NULL; //Error too few points
	}
	double mx, my, sx, sy, xmax,xmin, ymax,ymin,x,y;
	int ind = 0;
	int i_neg_eigenval;
	mx = 0;
	my = 0;
	xmax = 0;
	ymax = 0;
	xmin = FLT_MAX;
	ymin = FLT_MAX;
	for (std::list<Point*>::const_iterator i = edgeList.cbegin(); i != edgeList.cend(); i++)	{
		//(row,col)
		x = (*i)->getX();
		y = (*i)->getY();
		mx += x;
		my += y;
		if (x > xmax) {
			xmax = x;
		}
		if (x < xmin) {
			xmin = x;
		}
		if (y > ymax) {
			ymax = y;
		}
		if (y < ymin) {
			ymin = y;
		}
		ind++;
	}
	//normalize data
	//mx = mean(X);
	//my = mean(Y);
	//sx = (max(X) - min(X)) / 2;
	//sy = (max(Y) - min(Y)) / 2;
	mx /= edgeList.size();
	my /= edgeList.size();
	sx = (xmax - xmin) / 2;
	sy = (ymax - ymin) / 2;

#ifdef DEBUG_CALC
	cout << "mx= " << mx << endl << "my= " << my << "sx= " << sx << endl << "sy= " << sy << endl;
	cout << "maxx= " << xmax<< endl << "maxy= " << ymax << endl;
	cout << "minx= " << xmin << endl << "miny= " << ymin << endl;
#endif
	
	Eigen::MatrixXd D(edgeList.size(), 6);
	Eigen::MatrixXd C(6,6);
	Eigen::MatrixXd tmpA(3, 3);
	Eigen::MatrixXd tmpB(3, 3);
	Eigen::MatrixXd tmpD(3, 3);
	Eigen::MatrixXd tmpC(3, 3);
	Eigen::MatrixXd S, tmp, tmpE, tmp1, tmp2, evec_y;
	
	Eigen::VectorXd A(6);
	Eigen::VectorXd a(6);

	//% Build design matrix
	//D = [x.*x  x.*y  y.*y  x  y  ones(size(x))];
	int index = 0;
	for (std::list<Point*>::const_iterator i = edgeList.cbegin(); i != edgeList.cend(); i++)	{
		//D(row,col)
		//normalize step 2
		//x = (X - mx) / sx;
		//y = (Y - my) / sy;
		x = ((*i)->getX()-mx)/sx;
		y = ((*i)->getY()-my)/sy;
		D(index, 0) = x*x;
		D(index, 1) = x*y;
		D(index, 2) = y*y;
		D(index, 3) = x;
		D(index, 4) = y;
		D(index, 5) = 1;

		index++;
	}
	
	/*% Build scatter matrix
		S = D'*D;*/
	S = D.transpose()*D;

#ifdef DEBUG_CALC
	cout << "S=" << endl << S << endl << endl;
#endif
	/*% Build 6x6 constraint matrix
	C(6, 6) = 0; C(1, 3) = -2; C(2, 2) = 1; C(3, 1) = -2;*/
	C=C.Zero(6,6);
	C(0, 2) = -2;
	C(1, 1) = 1;
	C(2, 0) = -2;

	//% New way, numerically stabler in C[gevec, geval] = eig(S, C);
#ifdef DEBUG_CALC
	cout << "C=" << endl << C << endl << endl;
#endif
	/*% Break into blocks*/
	//tmpA = S(1:3, 1 : 3);
	tmpA(0, 0) = S(0, 0);		tmpA(0, 1) = S(0, 1);		tmpA(0, 2) = S(0, 2);
	tmpA(1, 0) = S(1, 0);		tmpA(1, 1) = S(1, 1);		tmpA(1, 2) = S(1, 2);
	tmpA(2, 0) = S(2, 0);		tmpA(2, 1) = S(2, 1);		tmpA(2, 2) = S(2, 2);

	//tmpB = S(1:3, 4 : 6);
	tmpB(0, 0) = S(0, 3);		tmpB(0, 1) = S(0, 4);		tmpB(0, 2) = S(0, 5);
	tmpB(1, 0) = S(1, 3);		tmpB(1, 1) = S(1, 4);		tmpB(1, 2) = S(1, 5);
	tmpB(2, 0) = S(2, 3);		tmpB(2, 1) = S(2, 4);		tmpB(2, 2) = S(2, 5);

	//tmpC = S(4:6, 4 : 6);
	tmpC(0, 0) = S(3, 3);		tmpC(0, 1) = S(3, 4);		tmpC(0, 2) = S(3, 5);
	tmpC(1, 0) = S(4, 3);		tmpC(1, 1) = S(4, 4);		tmpC(1, 2) = S(4, 5);
	tmpC(2, 0) = S(5, 3);		tmpC(2, 1) = S(5, 4);		tmpC(2, 2) = S(5, 5);

	//tmpD = C(1:3, 1 : 3);
	tmpD(0, 0) = C(0, 0);		tmpD(0, 1) = C(0, 1);		tmpD(0, 2) = C(0, 2);
	tmpD(1, 0) = C(1, 0);		tmpD(1, 1) = C(1, 1);		tmpD(1, 2) = C(1, 2);
	tmpD(2, 0) = C(2, 0);		tmpD(2, 1) = C(2, 1);		tmpD(2, 2) = C(2, 2);

#ifdef DEBUG_CALC
	cout << "tmpA=" << endl << tmpA << endl << endl;
	cout << "tmpB=" << endl << tmpB << endl << endl;
	cout << "tmpC=" << endl << tmpC << endl << endl;
	cout << "tmpD=" << endl << tmpD << endl << endl;
#endif
	//tmpE = inv(tmpC)*tmpB';
	tmpE = tmpC.inverse()*tmpB.transpose();

#ifdef DEBUG_CALC
	cout << "tmpE=" << endl << tmpE << endl << endl;
#endif

	//[evec_x, eval_x] = eig(inv(tmpD) * (tmpA - tmpB*tmpE)); 
	tmp1 = tmpB*tmpE;
	tmp2 = tmpA - tmp1;
	tmp = tmpD.inverse()*tmp2;

#ifdef DEBUG_CALC
	cout << "tmp1=" << endl << tmp1 << endl << endl;
	cout << "tmp2=" << endl << tmp2 << endl << endl;
	cout << "tmp=" << endl << tmp << endl << endl;
#endif

	Eigen::EigenSolver<Eigen::MatrixXd> es(tmp);
	Eigen::MatrixXd eigenval=es.eigenvalues().real();
	Eigen::MatrixXd eigenvec = es.eigenvectors().real();

#ifdef DEBUG_CALC
	cout << "val=" << endl << eigenval << endl << endl;
	cout << "val rows=" << eigenval.rows() << endl << "val cols=" << eigenval.cols() <<  endl;
	cout << "vec=" << endl << eigenvec << endl << endl;
#endif

	//% Find the negative(as det(tmpD) < 0) eigenvalue
	//I = find(real(diag(eval_x)) < 1e-8 & ~isinf(diag(eval_x)));
	for (int i = 0; i < eigenval.rows(); i++)	{
		if (eigenval(i,0) < 0) {
			i_neg_eigenval = i;
			break;
		}
	}

#ifdef DEBUG_CALC
	cout << "Negative eigenvalue: " << eigenval(i_neg_eigenval, 0) << endl << endl;
#endif

	/*% Extract eigenvector corresponding to negative eigenvalue
		A = real(evec_x(:, I));*/
	Eigen::VectorXd at=eigenvec.col(i_neg_eigenval).real();

#ifdef DEBUG_CALC
	cout << "coresponding eigenvector: " << endl << at << endl << endl;
#endif

	/*% Recover the bottom half...
		evec_y = -tmpE * A;
	A = [A; evec_y];*/
	evec_y = -tmpE*at;
#ifdef DEBUG_CALC
	cout << " Evec_y: " << endl << evec_y << "  " << evec_y.rows() << "  " << evec_y.cols() << endl << endl;
#endif
	A << at(0), at(1), at(2), evec_y(0), evec_y(1), evec_y(2);
#ifdef DEBUG_CALC
	cout << " A: " << endl << A << endl << endl;
#endif

	/*% unnormalize
		par = [
			A(1)*sy*sy, ...
				A(2)*sx*sy, ...
				A(3)*sx*sx, ...
				- 2 * A(1)*sy*sy*mx - A(2)*sx*sy*my + A(4)*sx*sy*sy, ...
				- A(2)*sx*sy*mx - 2 * A(3)*sx*sx*my + A(5)*sx*sx*sy, ...
				A(1)*sy*sy*mx*mx + A(2)*sx*sy*mx*my + A(3)*sx*sx*my*my   ...
				- A(4)*sx*sy*sy*mx - A(5)*sx*sx*sy*my   ...
				+ A(6)*sx*sx*sy*sy   ...
		]';*/
	a(0) =A(0)* sy*sy;
	a(1) = A(1)*sx*sy;
	a(2) = A(2)*sx*sx;
	a(3) = -2 * A(0)*sy*sy*mx - A(1)*sx*sy*my + A(3)*sx*sy*sy;
	a(4) = -A(1)*sx*sy*mx - 2 * A(2)*sx*sx*my + A(4)*sx*sx*sy;
	a(5) = A(0)*sy*sy*mx*mx + A(1)*sx*sy*mx*my + A(2)*sx*sx*my*my- A(3)*sx*sy*sy*mx - A(4)*sx*sx*sy*my+ A(5)*sx*sx*sy*sy;

#ifdef DEBUG_CALC
	cout << " a: " << endl << a << endl << endl;
#endif

	//calculate if the ellipse is a true ellipse for the given pixels
	int n_points = edgeList.size();
	double dist;
	double sum_dist=0;

	//for (std::list<Point*>::const_iterator i = edgeList.cbegin(); i != edgeList.cend(); i++)	{
	//	//calc algebraic distance to ellipse
	//	x = (*i)->getX();
	//	y = (*i)->getY();
	//	dist = abs(a(0)*x*x + a(1)*x*y + a(2)*y*y + a(3)*x + a(4)*y + a(5));
	//	cout << ": " << dist << endl << endl;
	//	sum_dist += dist;
	//}
	//cout << "Summe der Abweichungen: "<<sum_dist << endl << endl;
	Ellipse *e=new Ellipse(a(0), a(1), a(2), a(3), a(4), a(5));
	e->calcSumOfDistances(&edgeList);
	return e;

}


