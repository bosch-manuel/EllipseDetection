#include "PreProcessing.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "Point.h"
#include <list>
#include <set>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm> 
#include <ctime>
#include <stdio.h>
#include "Defines.h"
#include "EdgeSegment.h"


#ifdef DEBUG
std::fstream f;
//char *log = "..\\PreProcessing_Debug.txt";
#endif
int edgeDetection(cv::Mat input_image, cv::Mat edge_image, int lowThreshold, int highThreshold, int kernelSize) {
	cv::Canny(input_image, edge_image, lowThreshold, highThreshold, kernelSize);

	return 0;
}

/*Removes needless pixels in a 2x2 neighbourhood, so that an edge in this area will have a width of only one pixel
param:
r	row of the upper left pixel
c	column of the upper left pixel*/
void edgeThinning(int r, int c, cv::Mat *image) {
	uchar p[4]; //represents the values of pixels in the 2x2 neightbourhood
	if (r + 1 < image->rows && c + 1 < image->cols) {
		p[0] = image->at<uchar>(r, c);
		p[1] = image->at<uchar>(r, c + 1);
		p[2] = image->at<uchar>(r + 1, c);
		p[3] = image->at<uchar>(r + 1, c + 1);

		if (p[0] && p[1] && p[2] && p[3]) { //case e
			image->at<uchar>(r + 1, c) = 0;
			image->at<uchar>(r + 1, c + 1) = 0;
		}
		else if (p[0] && p[1] && p[2]) { //case a
			image->at<uchar>(r, c) = 0;
		}
		else if (p[0] && p[1] && p[3]) { //case b
			image->at<uchar>(r, c + 1);
		}
		else if (p[0] && p[2] && p[3]) { //case c
			image->at<uchar>(r + 1, c) = 0;
		}
		else if (p[0] && p[1] && p[2]) { //case d
			image->at<uchar>(r + 1, c + 1) = 0;
		}
	}
}

int checkNeighbors(int r, int c, cv::Mat *edgeImage) {
	int r1, r2, r3, c1, c2, c3, crossings, rows, cols;
	// | 1 | 8 | 7 |
	//	--- --- ---
	// | 2 | x | 6 |
	//  --- --- ---
	// | 3 | 4 | 5 |
	uchar a[8] = { 0 };
	uchar b[8] = { 0 };
	crossings = 0;
	rows = edgeImage->rows;
	cols = edgeImage->cols;
	//check out the neighborhood
	r1 = r - 1; r2 = r; r3 = r + 1;
	c1 = c - 1; c2 = c; c3 = c + 1;
	//first row
	if (r1 > -1) {
		if ((c1 > -1) && edgeImage->at<uchar>(r1, c1) > 0){
			a[0] = 1;
			b[7] = 1;
		}
		if (edgeImage->at<uchar>(r1, c2) > 0){
			a[7] = 1;
			b[6] = 1;
		}
		if ((c3 < cols) && edgeImage->at<uchar>(r1, c3)>0){
			a[6] = 1;
			b[5] = 1;
		}
	}

	if ((c1 > -1) && edgeImage->at<uchar>(r, c1) > 0){
		a[1] = 1;
		b[0] = 1;
	}
	if ((c3 < cols) && edgeImage->at<uchar>(r, c3)>0){
		a[5] = 1;
		b[4] = 1;
	}

	if (r3<rows) {
		if ((c1 > -1) && edgeImage->at<uchar>(r3, c1) > 0){
			a[2] = 1;
			b[1] = 1;
		}
		if (edgeImage->at<uchar>(r3, c2) > 0){
			a[3] = 1;
			b[2] = 1;
		}
		if ((c3 < cols) && edgeImage->at<uchar>(r3, c3)>0){
			a[4] = 1;
			b[3] = 1;
		}
	}

	for (int i = 0; i < 8; i++){
		crossings += std::abs(a[i] - b[i]);
	}
	//2 crossings = center point is an end point
	//>5 crossings =center point is a junction point
	return crossings;
}

/*Removes a Junction an adds all pixel above it as end point*/
int removeJunction(int r, int c, cv::Mat *edgeImage, std::list<Point*> *endPoints) {
	int r1, r2, r3, c1, c2, c3, rows, cols, ends;
	int value = 255;
	ends = 0;
	rows = edgeImage->rows;
	cols = edgeImage->cols;
	//check out the neighborhood
	r1 = r - 1; r2 = r; r3 = r + 1;
	c1 = c - 1; c2 = c; c3 = c + 1;
	//first row
	if (r1 > -1) {
		if ((c1 > -1) && edgeImage->at<uchar>(r1, c1) == EDGEPOINT){
			endPoints->push_back(new Point(r1, c1));
			edgeImage->at<uchar>(r1, c1) = ENDPOINT;
			//endPointsImage.at<uchar>(r1, c1)=value;
#ifdef DEBUG
			f << "End Point:  " << r1 << " , " << c1 << std::endl;
#endif
			ends++;
		}
		if (edgeImage->at<uchar>(r1, c2) == EDGEPOINT){
			endPoints->push_back(new Point(r1, c2));
			edgeImage->at<uchar>(r1, c2) = ENDPOINT;
			//endPointsImage.at<uchar>(r1, c2) = value;
#ifdef DEBUG
			f << "End Point:  " << r1 << " , " << c2<< std::endl;
#endif
			ends++;
		}
		if ((c3 < cols) && edgeImage->at<uchar>(r1, c3) == EDGEPOINT){
			endPoints->push_back(new Point(r1, c3));
			edgeImage->at<uchar>(r1, c3) = ENDPOINT;
			//endPointsImage.at<uchar>(r1, c3) = value;
#ifdef DEBUG
			f << "End Point:  " << r1 << " , " << c3 << std::endl;
#endif
			ends++;
		}
	}

	if ((c1 > -1) && edgeImage->at<uchar>(r, c1) == EDGEPOINT){
		endPoints->push_back(new Point(r, c1));
		edgeImage->at<uchar>(r, c1) = ENDPOINT;
#ifdef DEBUG
		f << "End Point:  " << r << " , " << c1 << std::endl;
#endif
		//endPointsImage.at<uchar>(r, c1) = value;
		ends++;
	}
	return ends;
}

int findEnds(std::list<Point*> *endPoints, cv::Mat *edgeImage, int* edgeCnt) {
#ifdef DEBUG
	f.open(FINDENDS_DEBUG,std::ios::out);
#endif

	int value = 255;
	int rows, cols;
	rows = edgeImage->rows;
	cols = edgeImage->cols;
	int crossings = 0;
	int ends = 0;
	int junctions = 0;

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			//thinn part of the edge
			edgeThinning(r, c, edgeImage);
			//(r,c) is an end point if within his 3x3 neighborhood exactly 2 pixels are set
			if (edgeImage->at<uchar>(r, c) > 0) {
				*edgeCnt = *edgeCnt + 1;
				crossings = checkNeighbors(r, c, edgeImage);
#ifdef DEBUG
				//printf("Gesetzt in Nachbarschaft von (%d,%d): %d \n",r,c, nSet);
				f << "Crossings in Nachbarschaft von" << r << "  " << c << "  " << crossings << std::endl;
#endif
				if (crossings == 0) {//isolated pixel, remove it
					edgeImage->at<uchar>(r, c) = 0;
					*edgeCnt = *edgeCnt - 1;
#ifdef DEBUG
					//printf("Isoliertes Pixel\n");
					f << "Isoliertes Pixel" << std::endl;
#endif
				}
				else if (crossings == 2) {//end point found, save it
					//endPointsImage.at<uchar>(r, c)=value;
					edgeImage->at<uchar>(r, c) = ENDPOINT;
					endPoints->push_back(new Point(r, c));
#ifdef DEBUG
					f << "End Point:  " << r << " , " << c << std::endl;
#endif
					ends++;
				}
				else if (crossings > 5) {//junction, remove it and add adjacent pixels as endpoints
					edgeImage->at<uchar>(r, c) = 0;
					*edgeCnt = *edgeCnt - 1;
					junctions++;
#ifdef DEBUG
					//printf("Junction Removed at (%d,%d) \n",r,c);
					f << "Junction Removed  " << r << " , " << c << std::endl;
#endif
					ends += removeJunction(r, c, edgeImage, endPoints);
				}
				crossings = 0;
			}
		}
	}

#ifdef DEBUG
	std::list<Point*>::iterator it;
	f << "############"<<std::endl<< "Ends: "<<ends<<" Juctions: "<<junctions << std::endl;
	for ( it =endPoints->begin() ; it != endPoints->end(); it++)	{
		f << (*it)->getY() << ", " << (*it)->getX() << std::endl;
	}
	f.close();	
#endif

	return ends;
}
/*Searches the next edge point to Point(row,col)
return	0 if a point was found, r and c contain the coordinates of the next point;
-1 when no point was found*/
int getNextPoint(int *row, int *col, cv::Mat *edgeImage) {
	int r, c, r1, r2, r3, c1, c2, c3, ret, rows, cols;
	ret = -1;
	r = *row;
	c = *col;
#ifdef DEBUG
	f  << "GetNextPoint für: " << r << ", " << c << std::endl;
#endif
	rows = edgeImage->rows;
	cols = edgeImage->cols;
	//check out the neighborhood
	r1 = r - 1; r2 = r; r3 = r + 1;
	c1 = c - 1; c2 = c; c3 = c + 1;
#ifdef DEBUG
	f <<   "		Punkte: " << "(" << r1 << ", " << c1 << ")" << "(" << r1 << ", " << c2 << ")" << "(" << r1 << ", " << c3 << ")" << std::endl
		<< "		(" << r2 << ", " << c1 << ")" << "(" << r2 << ", " << c2 << ")" << "(" << r2 << ", " << c3 << ")" << std::endl
		<< "		(" << r3 << ", " << c1 << ")" << "(" << r3 << ", " << c2 << ")" << "(" << r3 << ", " << c3 << ")"  << std::endl;
#endif
	//first row
	if (r1 > -1) {
		if ((c1 > -1) && edgeImage->at<uchar>(r1, c1) > 0){
			ret = 0;
			*row = r1;
			*col = c1;
#ifdef DEBUG
			f << "Nächster Punkt: " << *row << ", " << *col << std::endl;
#endif
			return 0;
		}
		if (edgeImage->at<uchar>(r1, c2) > 0){
			ret = 0;
			*row = r1;
			*col = c2;
#ifdef DEBUG
			f << "Nächster Punkt: " << *row << ", " << *col << std::endl;
#endif
			return 0;
		}
		if ((c3 < cols) && edgeImage->at<uchar>(r1, c3)>0){
			ret = 0;
			*row = r1;
			*col = c3;
#ifdef DEBUG
			f << "Nächster Punkt: " << *row << ", " << *col << std::endl;
#endif
			return 0;
		}
	}
	if ((c1 > -1) && edgeImage->at<uchar>(r, c1) > 0){
		ret = 0;
		*row = r;
		*col = c1;
#ifdef DEBUG
		f << "Nächster Punkt: " << *row << ", " << *col << std::endl;
#endif
		return 0;
	}
	if ((c3 < cols) && edgeImage->at<uchar>(r, c3)>0){
		ret = 0;
		*row = r;
		*col = c3;
#ifdef DEBUG
		f << "Nächster Punkt: " << *row << ", " << *col << std::endl;
#endif
		return 0;
	}
	if (r3<rows) {
		if ((c1 > -1) && edgeImage->at<uchar>(r3, c1) > 0){
			ret = 0;
			*row = r3;
			*col = c1;
#ifdef DEBUG
			f << "Nächster Punkt: " << *row << ", " << *col << std::endl;
#endif
			return 0;
		}
		if (edgeImage->at<uchar>(r3, c2) > 0){
			ret = 0;
			*row = r3;
			*col = c2;
#ifdef DEBUG
			f << "Nächster Punkt: " << *row << ", " << *col << std::endl;
#endif
			return 0;
		}
		if ((c3 < cols) && edgeImage->at<uchar>(r3, c3)>0){
			ret = 0;
			*row = r3;
			*col = c3;
#ifdef DEBUG
			f << "Nächster Punkt: " << *row << ", " << *col << std::endl;
#endif
			return 0;
		}
	}

	return ret;
}

int edgeLinking(cv::Mat *edgeImage, std::list<Point*> *endPoints, std::list<EdgeSegment*> *segments, int* edgeCnt) {
#ifdef DEBUG
	f.open(EDGE_DEBUG, std::ios::out);
#endif
	//first step: connect all known end points to edge segments
	std::list<Point*>::iterator it;
	int r, c, nSegs;
	EdgeSegment *es;
	nSegs = 0;
	for (it = endPoints->begin(); it != endPoints->end(); it++)	{
		r = (*it)->getY();
		c = (*it)->getX();
		//check if point is not already connected (=still set in edgeImage)
		if (edgeImage->at<uchar>(r, c) > 0) {
#ifdef DEBUG
			f <<std::endl<< "Neues Segment ab: " << r << ", " << c << std::endl;
#endif
			es = new EdgeSegment();
			nSegs++;
			//trace until the end of the edge
			do {
#ifdef DEBUG
				f << " -->(" << r << "," << c <<") ";
#endif
				es->push_backPoint(new Point(r, c));//add point to current segment
				edgeImage->at<uchar>(r, c) = 0; //delet current point
				*edgeCnt = *edgeCnt - 1;
			} while (!getNextPoint(&r, &c, edgeImage));
			segments->push_back(es);//end of edge reached; segment complete
		}

	}

#ifdef DEBUG
	f <<std::endl<< "Alle Endpunkte zugeordnet, Suche nach isolierten loops" << std::endl;
#endif

	//second step: search for any unattached edge points and connect them to segments. These correspond to isolated loops
	for (int row = 0; row < edgeImage->rows; row++){
		for (int col = 0; col < edgeImage->cols; col++) {
			//check if point is and edge point
			if (edgeImage->at<uchar>(row, col)>0) {
#ifdef DEBUG
				f << std::endl<<"Neues Segment ab: " << row << ", " << col << std::endl;
#endif
				es = new EdgeSegment();
				nSegs++;
				r = row;
				c = col;
				//trace until the end of the edge
				do {
#ifdef DEBUG
					f << " -->(" << r << "," << c << ") ";
#endif
					es->push_backPoint(new Point(r, c));//add point to current segment
					edgeImage->at<uchar>(r, c) = 0; //delet current point
					*edgeCnt = *edgeCnt - 1;
				} while (!getNextPoint(&r, &c, edgeImage));
				segments->push_back(es);//end of edge reached; segment complete
			}
			if (*edgeCnt < 1) {
				return nSegs;
			}
		}
	}

#ifdef DEBUG
	f.close();
#endif

	return nSegs;
}


/*Test whether the segment should be splitted at P inregard to the length condition
param
L1		point left to P
R1		point right to P
return
true	segment must be splitted at P
false	else*/
bool lengthCond(Point* L1, Point *P, Point *R1) {
	Point *PL1, *PR1;
	double lengthPL1, lengthPR1;
	PL1 = &(*P - *L1);
	PR1 = &(*P - *R1);
	lengthPL1 = PL1->norm();
	lengthPR1 = PR1->norm();
	if (lengthPL1 > LTH* lengthPR1 || lengthPR1 > LTH* lengthPL1) {
		return true;
	}
	else {
		return false;
	}
}

/*Test whether the segment should be splitted at P in regard to the curvature condition
param
L2		point before L1
L1		point before P
P		point before R1
R1		point after P
return
true	segment should be splitted at P
fase	else
*/
bool curvatureCond(Point *L2, Point *L1, Point *P, Point *R1) {
	Point *r0, *r1, *r2;
	double a1, a2, g1;
	int d;

	r0 = &(*L1 - *L2);
	r1 = &(*P - *L2);
	r2 = &(*R1 - *L2);
	a1 = acos((*r0 * *r1) / (r0->norm()* r1->norm())) * 100;
	a2 = acos((*r0 * *r2) / (r0->norm()* r2->norm())) * 100;
	g1 = acos((*r1 * *r2) / (r1->norm()* r2->norm())) * 100;

	d = (abs(a2 - a1) - g1) + .5;
	if (d != 0 || (a2 - a1) < A_TOL*100) {
		return true;
	}
	else {
		return false;
	}
}

/*Test whetherthe segment should be splitted at P in regard to the curvature condition
param
L2		point before L1
L1		point before P
P		point before R1
R1		point after P
R2		point after R1
return
true	segment should be splitted at P
fase	else
*/
bool angleCond(Point *L2, Point *L1, Point *P, Point *R1, Point *R2) {
	Point *r6, *r7, *r8, *r9;
	double b3, b1, b2;
	r6 = &(*L2 - *L1);
	r9 = &(*R2 - *R1);
	if (P != NULL) {
		r7 = &(*P - *L1);
		r8 = &(*P - *R1);
	}
	else {
		r7 = r6;
		r8 = r9;
	}

	b3 = acos((*r6 * *r7) / (r6->norm()* r7->norm()));
	b1 = acos((*r8 * *r9) / (r8->norm()* r9->norm()));
	b2 = acos((*r7 * *r8) / (r7->norm()* r8->norm()));

	if (abs(b1 - b2) > TH && abs(b3 - b2) > TH) {
		return true;
	}
	else {
		return false;
	}
}


int curveSegmentation(std::list<EdgeSegment*> *edgeSegs, std::list<EdgeSegment*> *curveSegs) {
	std::fstream csf;
#ifdef DEBUB_CURVE_SEG
	
	csf.open(CURVE_SEG_DEBUG, std::ios::out);
#endif 

	//check every edge segment
	int nCurvSegs = 0;
	for (std::list<EdgeSegment*>::const_iterator i = edgeSegs->begin(); i != edgeSegs->end(); i++) {
#ifdef IMPROVED_SEGMENTATION
		nCurvSegs += (*i)->curveSegmentationImproved(curveSegs, &csf);
#else
		//(*i)->evaluateCurvature(&ccf);
		nCurvSegs += (*i)->curveSegmentation(curveSegs, &csf);
#endif
	}

#ifdef DEBUB_CURVE_SEG
	csf.close();
#endif

	return nCurvSegs;
}

/*Connects two curve segments and groups them in one elliptical arc
if the segmenst are still grouped in a elliptical arc, the arcs will be joined
param:
cS1		curve segment 1
cS2		curve segment 2
arcs	set of existing elliptical arcs
*/
void connectSegments(EdgeSegment* cS1, EdgeSegment* cS2, std::set<EllipticalArc*> *arcs) {
	EllipticalArc* arcS1 = NULL, *arcS2 = NULL;
	for (std::set<EllipticalArc*>::iterator i = arcs->begin(); i != arcs->end(); i++)	{
		if ((*i)->containsSegment(cS1)) {
			arcS1 = (*i);
		}
		else if ((*i)->containsSegment(cS2)) {
			arcS2 = (*i);
		}
	}

	//check if segments are still grouped 
	if (arcS1 == NULL && arcS2 == NULL) {
		//create new elliptical arc and add both segments
		arcS1 = new EllipticalArc();
		if (cS1 != NULL) {
			arcS1->addSegment(cS1);
		}
		if (cS2 != NULL) {
			arcS1->addSegment(cS2);
		}

		arcs->insert(arcS1);
	}
	else if (arcS1 != NULL && arcS2 != NULL && arcS1 != arcS2) {
		//both segments are already a part of different arcs, so join them
		arcS1->joinArcs(arcS2);
		arcs->erase(arcS2);
		delete(arcS2);
	}
	else if (arcS1 != NULL && cS2 != NULL) {
		//only segment cS1 is part of an arc, add cS2
		arcS1->addSegment(cS2);
	}
	else if (arcS2 != NULL && cS2 != NULL) {
		arcS2->addSegment(cS1);
	}
}

////search for every m-th curve segment the n-th curve segment that has the min difference of tangents at their end points
/*
	return
		index of seg with min diff of tangents (-1 if no segment was found)*/
int findSegWithMinTangDiff(EdgeSegment *seg, std::vector<EdgeSegment*> *segments, int size) {
	Point *mfirst, *mend, *nfirst, *nend, *N1 = NULL, *M1 = NULL, *N2 = NULL, *M2 = NULL, *r1 = NULL, *r2 = NULL;
	EdgeSegment *cS_min = NULL,*m=NULL;
	int d, s = 0;
	int nEmE, nEmB, nBmE, nBmB,index_min=-1;


	double a_min = 2 * PI, a_tmp;
	nfirst = seg->getFirstPoint();
	nend = seg->getLastPoint();
	for (int i = 0; i < size;i++) {
		m = segments->at(i);
		if (seg != m && m!=NULL) {
			mfirst = m->getFirstPoint();
			mend = m->getLastPoint();
			//calc distance between n and m
			nEmE/*mEnE*/ = (*mend - *nend).norm() + .5;
			nBmE/*mEnB*/ = (*mend - *nfirst).norm() + .5;
			nEmB/*mBnE*/ = (*mfirst - *nend).norm() + .5;
			nBmB/*mBnB*/ = (*mfirst - *nfirst).norm() + .5;
			d = std::min(std::min(nEmE, nBmE), std::min(nEmB, nBmB));
			if (d > 0 && d < D0) {
				if (d == nEmE) {
					M1 = m->getLastPoint();
					N1 = seg->getLastPoint();
					M2 = m->getNextToLastPoint();
					N2 = seg->getNextToLastPoint();
				}
				else if (d == nBmE) {
					M1 = m->getLastPoint();
					N1 = seg->getFirstPoint();
					M2 = m->getNextToLastPoint();
					N2 = seg->getSecondPoint();
				}
				else if (d == nEmB) {
					M1 = m->getFirstPoint();
					N1 = seg->getLastPoint();
					M2 = m->getSecondPoint();
					N2 = seg->getNextToLastPoint();
				}
				else if (d == nBmB) {
					M1 = m->getFirstPoint();
					N1 = seg->getFirstPoint();
					M2 = m->getSecondPoint();
					N2 = seg->getSecondPoint();
				}

				//calc angle between the tangents of m and n
				r1 = &(*M1 - *M2);
				r2 = &(*N2 - *N1);
				a_tmp = acos((*r1 * *r2) / (r1->norm()* r2->norm()));
#ifdef DEBUB_CURVE_GRP
				csf << "Abstand: " << d << "  Winkel an Enden: M1(" << M1->getX() << "," << M1->getY() << ") M2(" << M2->getX() << "," << M2->getY() << ") und N1(" << N1->getX() << "," << N1->getY() << ") N2(" << N2->getX() << "," << N2->getY() << ") ::" << a_tmp << std::endl;
#endif
				if (a_tmp < a_min) {
					//current angle between n and m is the smallest so far, so keep it in mind
					a_min = a_tmp;
					index_min = i;
				}
			}
		}
	}

	return index_min;
}

/* Test whether tow curvesegments could match an ellipse in regard to global curve grouping condition*/
bool couldMakeEllipse(EdgeSegment* m,Point *P_m, EdgeSegment *n,Point *P_n) {
	double y, x;
	double d_P_m_P_n, d_P_m_C_n, d_P_n_C_m;
	if (m->getType() != CURVESEG || n->getType() != CURVESEG) {
		return false;
	}
	//calc C_m, C_n
	y = (m->getFirstPoint()->getY() + m->getLastPoint()->getY()) / 2;
	x = (m->getFirstPoint()->getX() + m->getLastPoint()->getX()) / 2;
	Point C_m(y, x);

	y = (n->getFirstPoint()->getY() + n->getLastPoint()->getY()) / 2;
	x = (n->getFirstPoint()->getX() + n->getLastPoint()->getX()) / 2;
	Point C_n(y, x);

	//calc distances
	d_P_m_P_n = sqrt(pow(P_m->getY()-P_n->getY(),2)+pow(P_m->getX()-P_n->getX(),2));
	d_P_m_C_n = sqrt(pow(P_m->getY() - C_n.getY(), 2) + pow(P_m->getX() - C_n.getX(), 2));
	d_P_n_C_m = sqrt(pow(P_n->getY() - C_m.getY(), 2) + pow(P_n->getX() - C_m.getX(), 2));

	if ((d_P_m_P_n > d_P_m_C_n) && (d_P_m_P_n > d_P_n_C_m)) {//global curve grounping cond 
		return true;
	}

	return false;
}

int fitEllipses(std::list<EdgeSegment*> *segments, std::list<Ellipse*> *ellipses) {
	int j = 0, a = 0,idx;
	int size = segments->size(),size_i=0,size_j=0,size_k=0;
	int n_tries = 0;//number of tries for estimation of consensus set for 
	int max_tries;
	Ellipse *eS1=NULL;
	double fitting_factor_i = 0,fitting_factor_j=0,fitting_factor_k=0;
	bool s1_isValid = false;
	EdgeSegment *cur_Seg = NULL;
	EdgeSegment *segi=NULL, *segj=NULL, *segk=NULL;

	std::vector<EdgeSegment*> segs(size);
	std::vector<Point*> *pt;
	std::vector<int> *indexesOfMatchingPoints_i, *indexesOfMatchingPoints_j, *indexesOfMatchingPoints_k;
	std::vector<Point*> consesusSet;
	std::vector<std::vector<Point*>*> seg_points(size);
	std::vector<Point*> randomEdgePixels(N_POINTS);

	for (std::list<EdgeSegment*>::iterator i = segments->begin(); i !=segments->end(); i++) {
		segs[j] = (*i);

		//get segment points and store them into array
		seg_points[j] = new std::vector<Point*>((*i)->getLength());
		a = 0;
		for (std::list<Point*>::const_iterator b = (*i)->cbegin(); b != (*i)->cend(); b++){
			seg_points[j]->at(a) = (*b);
			a++;
		}

		j++;
	}

	std::srand(unsigned(std::time(0)));
#ifdef DEBUG_RANSAC
	cv::Mat ellipseImage(640, 480, CV_8UC3); ellipseImage = cv::Scalar(255, 255, 255);
	char* EllipseOnSource_window = "Ellipse";
#endif
	for (int i = 0; i < size; i++) {
		if (segs[i]!=NULL) {
#ifdef DEBUG_RANSAC
			std::cout << "##############################"<<std::endl<<"Betrachtung fuer Segment:" << segs[i]->ID<< std::endl << std::endl;
			ellipseImage = cv::Scalar(255, 255, 255);
#endif
			size_i = seg_points[i]->size();
			size_j = 0;
			cur_Seg = segs[i];
			//get the next segment which could be a part of the same ellipse (global curve grounping conditions)
			for (size_t j = 0; j < segs.size(); j++)	{
				if (segs[i] != segs[j] && segs[j]!=NULL) {
					size_j = seg_points[j]->size();
					Point* P_i = seg_points[i]->at(seg_points[i]->size()/2); //get P_i
					Point *P_j = seg_points[j]->at(seg_points[j]->size() / 2);//get P_j
					if ((size_i+size_j)>5 && couldMakeEllipse(segs[i], P_i, segs[j], P_j)) {
						//calc ellipse using ransac
						s1_isValid = false;
						n_tries = 0;
						max_tries = (seg_points[i]->size() + seg_points[j]->size()) / 2;
						while (!s1_isValid && n_tries < 5) {
							//get 6 random points from cur_seg and minG_seg 
							//array with indexes from 0 - cur_seg.size()+minG_seg.size()-1
							//#ifdef DEBUG_RANSAC
							//				using namespace std;
							//				std::srand(unsigned(std::time(0)));
							//				std::cout << "Zufällige Indizes ziehen:" << std::endl << std::endl;
							//#endif
							std::vector<int> indexes(size_i + size_j);
							for (int x = 0; x < indexes.size(); x++){
								indexes[x] = x;
							}
							std::random_shuffle(indexes.begin(), indexes.end());

							//#ifdef DEBUG_RANSAC
							//				using namespace std;
							//				for (int x = 0; x < indexes.size(); x++){
							//					cout << indexes[x] << ", ";
							//				}
							//				cout << endl << endl;
							//#endif
							//get random pixels S1
							for (int x = 0; x < N_POINTS &&x< indexes.size(); x++){
								idx = indexes[x];
								if (idx < size_i) {//take pixels from segs[i] (index i in seg_points)
									randomEdgePixels[x] = seg_points[i]->at(idx);
								}
								else { //take pixel from segs[j] (index j in seg_points)
									randomEdgePixels[x] = seg_points[j]->at(idx - size_i);
								}
							}

#ifdef DEBUG_RANSAC
							using namespace std;
							cout << "Zufällige Punkte:" << endl << endl;
							for (int i = 0; i < randomEdgePixels.size(); i++)	{
								cout << "(" << randomEdgePixels[i]->getX() << "," << randomEdgePixels[i]->getY() << ") ";
							}
#endif

//#ifdef DEBUG_RANSAC
//							ellipseImage = cv::Scalar(255, 255, 255);
//							segs[i]->drawToImage(&ellipseImage, cv::Vec3d(0, 255, 0));
//							if (j != -1)
//								segs[j]->drawToImage(&ellipseImage, cv::Vec3d(255, 0, 0));
//							cv::namedWindow(EllipseOnSource_window, CV_WINDOW_AUTOSIZE);
//							cv::imshow(EllipseOnSource_window, ellipseImage);
//							cv::waitKey(0);
//#endif
							//calculate best fitting ellipse for all points in randomEdgePixels

							eS1 = calcEllipse(&randomEdgePixels);

							//estimate consensus sets S1* for both segments
							indexesOfMatchingPoints_i = eS1->getIndexesOfMatchingPoints(seg_points[i]);
							fitting_factor_i = indexesOfMatchingPoints_i->size() / size_i;
							indexesOfMatchingPoints_j = eS1->getIndexesOfMatchingPoints(seg_points[j]);
							fitting_factor_j = indexesOfMatchingPoints_j->size() / size_j;
#ifdef DEBUG_RANSAC
							using namespace std;
							cout << "Übereinstimmung von " << segs[i]->ID << ": " << fitting_factor_i << endl << endl;
							cout << "Übereinstimmung von " << segs[j]->ID << ": " << fitting_factor_j << endl << endl;
#endif
							if (fitting_factor_i >= TH_PARTIALFIT && fitting_factor_j >= TH_PARTIALFIT) { //TH_PRATIALFIT % of the points from an segment must match the ellipse to have a valid consensus set
								s1_isValid = true;
								//if TH_FULLFIT % of the points match the calculated ellipse e, this curve segement will not be included in further calculations
								if (fitting_factor_i >= TH_FULLFIT) {
									segi = segs[i];
									segs[i] = NULL; //mark segment as assinged to an ellipse 
									//add points to the consensus set
									for (std::vector<int>::const_iterator it = indexesOfMatchingPoints_i->cbegin(); it != indexesOfMatchingPoints_i->cend(); it++){
										consesusSet.push_back(seg_points[i]->at(*it));
									}
								}
								//else {
								//	//add points and remove them from segment
								//	for (std::vector<int>::const_iterator it = indexesOfMatchingPoints_i->cbegin(); it != indexesOfMatchingPoints_i->cend(); it++){
								//		consesusSet.push_back(seg_points[i]->at(*it));
								//		seg_points[i]->erase(seg_points[i]->begin() + *it);
								//	}
								//}
								if (fitting_factor_j >= TH_FULLFIT) {
									segj = segs[j];
									segs[j] = NULL; //mark segment as assinged to an ellipse 
									//add points to the consensus set
									for (std::vector<int>::const_iterator it = indexesOfMatchingPoints_j->cbegin(); it != indexesOfMatchingPoints_j->cend(); it++){
										consesusSet.push_back(seg_points[j]->at(*it));
									}
								}
								//else {
								//	//add points and remove them from segment
								//	for (std::vector<int>::const_iterator it = indexesOfMatchingPoints_j->cbegin(); it != indexesOfMatchingPoints_j->cend(); it++){
								//		consesusSet.push_back(seg_points[j]->at(*it));
								//		seg_points[j]->erase(seg_points[j]->begin() + *it);
								//	}
								//}
							}
//#ifdef DEBUG_RANSAC
//							ellipseImage = cv::Scalar(255, 255, 255);
//							if (segs[i] != NULL && segs[j] != NULL) {
//								segs[i]->drawToImage(&ellipseImage, cv::Vec3d(0, 255, 0));
//								if (j != -1)
//									segs[j]->drawToImage(&ellipseImage, cv::Vec3d(255, 0, 0));
//								cv::namedWindow(EllipseOnSource_window, CV_WINDOW_AUTOSIZE);
//								cv::namedWindow(EllipseOnSource_window, CV_WINDOW_AUTOSIZE);
//								
//							}
//
//							eS1->drawToImage(&ellipseImage, new cv::Scalar(50, 50, 255));
//							cv::imshow(EllipseOnSource_window, ellipseImage);
//							cv::waitKey(0);
//							
//#endif
							//----------------- if there is a valid consensus set, continue the matching process, else calculate new ellipse with new random pixels
							n_tries++;
						}

						
						if (s1_isValid) {
#ifdef DEBUG_RANSAC
							eS1->drawToImage(&ellipseImage, new cv::Scalar(50, 50, 255));
							cv::imshow(EllipseOnSource_window, ellipseImage);
							cv::waitKey(0);

#endif
							//--- if a valid ellipse was found, search for all matching points from other segments which could make an ellipse
							for (size_t k = 0; k < segs.size(); k++)	{
								if (segs[k] != NULL) {
//#ifdef DEBUG_RANSAC
//									ellipseImage = cv::Scalar(255, 255, 255);
//									eS1->drawToImage(&ellipseImage, new cv::Scalar(50, 50, 255));
//									segs[k]->drawToImage(&ellipseImage, cv::Vec3d(255, 0, 0));
//									cv::namedWindow(EllipseOnSource_window, CV_WINDOW_AUTOSIZE);
//									cv::namedWindow(EllipseOnSource_window, CV_WINDOW_AUTOSIZE);
//									cv::imshow(EllipseOnSource_window, ellipseImage);
//									cv::waitKey(0);
//#endif
									size_k = seg_points[k]->size();
									Point *P_k = seg_points[k]->at(seg_points[k]->size() / 2);//get P_j
									if (couldMakeEllipse(cur_Seg, P_i, segs[k], P_k)) {
										indexesOfMatchingPoints_k = eS1->getIndexesOfMatchingPoints(seg_points[k]);
										fitting_factor_k = indexesOfMatchingPoints_k->size() / size_k;
#ifdef DEBUG_RANSAC
										using namespace std;
										cout << "Übereinstimmung von " << segs[k]->ID << ": " << fitting_factor_k << endl << endl;
#endif
										if (fitting_factor_k >= TH_FULLFIT) {
											segs[k] = NULL; //mark segment as assinged to an ellipse 
											//add points to the consensus set
											for (std::vector<int>::const_iterator it = indexesOfMatchingPoints_k->cbegin(); it != indexesOfMatchingPoints_k->cend(); it++){
												consesusSet.push_back(seg_points[k]->at(*it));
											}
										}
										//else if(fitting_factor_k>=TH_PARTIALFIT) {
										//	//add points and remove them from segment
										//	for (std::vector<int>::const_iterator it = indexesOfMatchingPoints_k->cbegin(); it != indexesOfMatchingPoints_k->cend(); it++){
										//		consesusSet.push_back(seg_points[k]->at(*it));
										//		seg_points[k]->erase(seg_points[k]->begin() + *it);
										//	}
										//}
									}
								}
							}

							//all matching points are found, calc the ellipse for all points in the consensus set
							eS1 = calcEllipse(&consesusSet);
#ifdef DEBUG_RANSAC
							segi->drawToImage(&ellipseImage, cv::Vec3d(0, 0, 0));
							segj->drawToImage(&ellipseImage, cv::Vec3d(0, 0, 0));
							eS1->drawToImage(&ellipseImage, new cv::Scalar(50, 50, 255));
							cv::namedWindow(EllipseOnSource_window, CV_WINDOW_AUTOSIZE);
							cv::imshow(EllipseOnSource_window, ellipseImage);
							cv::waitKey(0);
#endif
							ellipses->push_back(eS1);
							consesusSet.clear();
							break; // proceed with next segment
						}
					}
				}
			}
		}
	}

	//---- all segments should fitt an ellipse, only segments for which no matching segment was found remain ---- 
	for (int i = 0; i < size-1; i++) {
		if (segs[i] != NULL) {
			eS1=segs[i]->calcEllipse();
			if (eS1 != NULL) {
				ellipses->push_back(eS1);
#ifdef DEBUG_RANSAC
				eS1->drawToImage(&ellipseImage, new cv::Scalar(50, 50, 255));
				cv::namedWindow(EllipseOnSource_window, CV_WINDOW_AUTOSIZE);
				cv::imshow(EllipseOnSource_window, ellipseImage);
				cv::waitKey(0);
#endif
			}
		}
	}
	return 0;
}

//int curveGrouping(std::list<EdgeSegment*> *curveSegs, std::set<EllipticalArc*> *arcs) {
//#ifdef DEBUB_CURVE_GRP
//	std::fstream csf;
//	csf.open(CURVE_GRP_DEBUG, std::ios::out);
//#endif 
//	Point *nfirst = NULL, *nend = NULL, *mfirst = NULL, *mend = NULL, *N1 = NULL, *M1 = NULL, *N2 = NULL, *M2 = NULL, *r1 = NULL, *r2 = NULL;
//	Point *L1 = NULL, *L2 = NULL, *L3 = NULL, *R1 = NULL, *R2 = NULL, *R3 = NULL;
//	EdgeSegment *cS_min = NULL;
//	int order_min; //eS_min must be added in this order
//	int order_tmp;
//	int d, s = 0;
//	int nEmE, nEmB, nBmE, nBmB;
//	double a_min = 2 * PI, a_tmp;
//	//neightbourhood curve grouping:
//	//search for every m-th curve segment the n-th curve segment that has the min difference of tangents at their end points
//	for (std::list<EdgeSegment*>::iterator n = curveSegs->begin(); n != curveSegs->end(); n++) {
//		if (*n != NULL) {
//			/*EllipticalArc *tmp = new EllipticalArc;
//			tmp->addSegment((*n));
//			arcs->insert(tmp);*/
//#ifdef DEBUB_CURVE_GRP
//			csf << "Seg " << (*n)->ID << std::endl;
//#endif
//			nfirst = (*n)->getFirstPoint();
//			nend = (*n)->getLastPoint();
//			for (std::list<EdgeSegment*>::iterator m = curveSegs->begin(); m != curveSegs->end(); m++) {
//				if (*n != *m) {
//					mfirst = (*m)->getFirstPoint();
//					mend = (*m)->getLastPoint();
//					//calc distance between n and m
//					nEmE/*mEnE*/ = (*mend - *nend).norm() + .5;
//					nBmE/*mEnB*/ = (*mend - *nfirst).norm() + .5;
//					nEmB/*mBnE*/ = (*mfirst - *nend).norm() + .5;
//					nBmB/*mBnB*/ = (*mfirst - *nfirst).norm() + .5;
//					d = std::min(std::min(nEmE, nBmE), std::min(nEmB, nBmB));
//					if (d>0 && d < D0) {
//						if (d == nEmE) {
//							order_tmp = END_END;
//							M1 = (*m)->getLastPoint();
//							N1 = (*n)->getLastPoint();
//							M2 = (*m)->getNextToLastPoint();
//							N2 = (*n)->getNextToLastPoint();
//						}
//						else if (d == nBmE) {
//							order_tmp = BEGIN_END;
//							M1 = (*m)->getLastPoint();
//							N1 = (*n)->getFirstPoint();
//							M2 = (*m)->getNextToLastPoint();
//							N2 = (*n)->getSecondPoint();
//						}
//						else if (d == nEmB) {
//							order_tmp = END_BEGIN;
//							M1 = (*m)->getFirstPoint();
//							N1 = (*n)->getLastPoint();
//							M2 = (*m)->getSecondPoint();
//							N2 = (*n)->getNextToLastPoint();
//						}
//						else if (d == nBmB) {
//							order_tmp = BEGIN_BEGIN;
//							M1 = (*m)->getFirstPoint();
//							N1 = (*n)->getFirstPoint();
//							M2 = (*m)->getSecondPoint();
//							N2 = (*n)->getSecondPoint();
//						}
//
//						//calc angle between the tangents of m and n
//						r1 = &(*M1 - *M2);
//						r2 = &(*N2 - *N1);
//						a_tmp = acos((*r1 * *r2) / (r1->norm()* r2->norm()));
//#ifdef DEBUB_CURVE_GRP
//						csf << "Abstand: " << d << "  Winkel an Enden: M1(" << M1->getX() << "," << M1->getY() << ") M2(" << M2->getX() << "," << M2->getY() << ") und N1(" << N1->getX() << "," << N1->getY() << ") N2(" << N2->getX() << "," << N2->getY() << ") ::" << a_tmp << std::endl;
//#endif
//						if (a_tmp < a_min) {
//							//current angle between n and m is the smallest so far, so keep it in mind
//							order_min = order_tmp;
//							a_min = a_tmp;
//							cS_min = (*m);
//							L2 = M2;
//							L1 = M1;
//							R1 = N1;
//							R2 = N2;
//						}
//					}
//				}
//			}
//
//			//group n with the curve segment which has the smallest angle between tangents
//			if (cS_min != NULL) {
//				s = 0;
//				//get third point of cS_min in regard to connection order (order_min)
//				if (order_min == END_END || order_min==BEGIN_END){
//					for (std::list<Point*>::const_reverse_iterator i = cS_min->crbegin(); i != cS_min->crend() && s < 3; i++, s++)	{
//						L3 = (*i);
//					}
//				}
//				else if (order_min == BEGIN_BEGIN || order_min == END_BEGIN) {
//					for (std::list<Point*>::const_iterator i = cS_min->cbegin(); i != cS_min->cend() && s < 3; i++, s++)	{
//						L3 = (*i);
//					}
//				}
//				s = 0;
//				//get third point of n
//				if (order_min == END_END || order_min == END_BEGIN){
//					for (std::list<Point*>::const_reverse_iterator i = (*n)->crbegin(); i != (*n)->crend() && s < 3; i++, s++)	{
//						R3 = (*i);
//					}
//				}
//				else if (order_min == BEGIN_BEGIN || order_min == BEGIN_END) {
//					for (std::list<Point*>::const_iterator i = (*n)->cbegin(); i != (*n)->cend() && s < 3; i++, s++)	{
//						R3 = (*i);
//					}
//				}
//
//				if (angleCond(L3, L2, L1, R1, R2) || angleCond(R3, R2, R1, L1, L2) ||curvatureCond(R1,L1,L2,L3) || curvatureCond(L1,R1,R2,R3)) {
//					connectSegments((*n), NULL, arcs);// all segments must be contained in the arc segment set, even unconnected ones
//				}
//				else {
//					connectSegments((*n), cS_min, arcs);
//#ifdef DEBUB_CURVE_GRP
//					csf << "Seg " << (*n)->ID << " mit Seg " << cS_min->ID << "-> kleinster Winkel: " << a_min << std::endl;
//					csf << "################ Zusammengefuegt ######################" << std::endl;
//#endif
//				}
//			}
//			else {// all segments must be contained in the arc segment set, even unconnected ones
//				connectSegments((*n), NULL, arcs);
//			}
//			cS_min = NULL;
//			a_min = 2 * PI;
//		}
//
//	}
//
//	//global curve grouping:
//	for (std::set<EllipticalArc*>::iterator n = arcs->begin(); n != arcs->end(); n++) {
//		if (*n != NULL) {
//			for (std::list<EdgeSegment*>::iterator m = curveSegs->begin(); m != curveSegs->end(); m++) {
//
//			}
//		}
//	}
//
//#ifdef DEBUB_CURVE_GRP
//	csf.close();
//#endif
//
//	return arcs->size();
//}