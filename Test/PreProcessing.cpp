#include "PreProcessing.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "Point.h"
#include <list>
#include <set>
#include <fstream>
#include <algorithm> 
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
void edgeThinning(int r, int c,cv::Mat *image) {
	uchar p[4]; //represents the values of pixels in the 2x2 neightbourhood
	if (r + 1 < image->rows && c + 1 < image->cols) {
		p[0] = image->at<uchar>(r, c);
		p[1] = image->at<uchar>(r, c+1);
		p[2] = image->at<uchar>(r+1, c);
		p[3] = image->at<uchar>(r+1, c+1);

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

int checkNeighbors(int r, int c,cv::Mat *edgeImage) {
	int r1, r2, r3, c1, c2, c3, crossings,rows,cols;
	// | 1 | 8 | 7 |
	//	--- --- ---
	// | 2 | x | 6 |
	//  --- --- ---
	// | 3 | 4 | 5 |
	uchar a[8] = { 0 };
	uchar b[8] = {0};
	crossings = 0;
	rows = edgeImage->rows;
	cols = edgeImage->cols;
	//check out the neighborhood
	r1 = r - 1; r2 = r; r3 = r + 1;
	c1 = c - 1; c2 = c; c3 = c + 1;
	//first row
	if (r1>-1) {
		if ((c1 > -1) && edgeImage->at<uchar>(r1, c1)>0){
			a[0] = 1;
			b[7] = 1;
		}
		if (edgeImage->at<uchar>(r1, c2)>0){
			a[7] = 1;
			b[6] = 1;
		}
		if ((c3 < cols) && edgeImage->at<uchar>(r1, c3)>0){
			a[6] = 1;
			b[5] = 1;
		}
	}

	if ((c1 > -1) && edgeImage->at<uchar>(r, c1)>0){
		a[1] = 1;
		b[0] = 1;
	}
	if ((c3 < cols) && edgeImage->at<uchar>(r, c3)>0){
		a[5] = 1;
		b[4] = 1;
	}

	if (r3<rows) {
		if ((c1 > -1) && edgeImage->at<uchar>(r3, c1)>0){
			a[2] = 1;
			b[1] = 1;
		}
		if (edgeImage->at<uchar>(r3, c2)>0){
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
	int r1, r2, r3, c1, c2, c3, rows, cols,ends;
	int value = 255;
	ends = 0;
	rows = edgeImage->rows;
	cols = edgeImage->cols;
	//check out the neighborhood
	r1 = r - 1; r2 = r; r3 = r + 1;
	c1 = c - 1; c2 = c; c3 = c + 1;
	//first row
	if (r1>-1) { 
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

int findEnds(std::list<Point*> *endPoints, cv::Mat *edgeImage) {
#ifdef DEBUG
	f.open(FINDENDS_DEBUG,std::ios::out);
#endif

	int value = 255;
	int rows, cols;
	rows = edgeImage->rows;
	cols = edgeImage->cols;
	int crossings = 0;
	int ends=0;
	int junctions = 0;

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			//thinn part of the edge
			edgeThinning(r, c,edgeImage);
			//(r,c) is an end point if within his 3x3 neighborhood exactly 2 pixels are set
			if (edgeImage->at<uchar>(r, c) > 0) {
				crossings = checkNeighbors(r, c, edgeImage);
#ifdef DEBUG
				//printf("Gesetzt in Nachbarschaft von (%d,%d): %d \n",r,c, nSet);
				f << "Crossings in Nachbarschaft von" << r << "  " << c << "  " << crossings << std::endl;
#endif
				if (crossings == 0) {//isolated pixel, remove it
					edgeImage->at<uchar>(r, c) = 0;
#ifdef DEBUG
					//printf("Isoliertes Pixel\n");
					f << "Isoliertes Pixel" << std::endl;
#endif
				}else if (crossings == 2) {//end point found, save it
					//endPointsImage.at<uchar>(r, c)=value;
					edgeImage->at<uchar>(r, c) = ENDPOINT;
					endPoints->push_back(new Point(r, c));
#ifdef DEBUG
					f << "End Point:  " << r << " , " << c << std::endl;
#endif
					ends++;
				}else if (crossings > 5) {//junction, remove it and add adjacent pixels as endpoints
					edgeImage->at<uchar>(r, c) = 0;
					junctions++;
#ifdef DEBUG
					//printf("Junction Removed at (%d,%d) \n",r,c);
					f << "Junction Removed  " << r << " , " << c << std::endl;
#endif
					ends += removeJunction( r,  c, edgeImage, endPoints);
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
int getNextPoint(int *row, int *col,cv::Mat *edgeImage) {
	int r,c,r1, r2, r3, c1, c2, c3, ret, rows, cols;
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
	if (r1>-1) {
		if ((c1 > -1) && edgeImage->at<uchar>(r1, c1)>0){
			ret=0;
			*row = r1;
			*col = c1;
#ifdef DEBUG
			f << "Nächster Punkt: " << *row << ", " << *col << std::endl;
#endif
			return 0;
		}
		if (edgeImage->at<uchar>(r1, c2)>0){
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
	if ((c1 > -1) && edgeImage->at<uchar>(r, c1)>0){
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
		if ((c1 > -1) && edgeImage->at<uchar>(r3, c1)>0){
			ret = 0;
			*row = r3;
			*col = c1;
#ifdef DEBUG
			f << "Nächster Punkt: " << *row << ", " << *col << std::endl;
#endif
			return 0;
		}
		if (edgeImage->at<uchar>(r3, c2)>0){
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

int edgeLinking(cv::Mat *edgeImage, std::list<Point*> *endPoints, std::list<EdgeSegment*> *segments) {
#ifdef DEBUG
	f.open(EDGE_DEBUG, std::ios::out);
#endif
	//first step: connect all known end points to edge segments
	std::list<Point*>::iterator it;
	int r, c, nSegs;
	EdgeSegment *es;
	nSegs = 0;
	for (it=endPoints->begin(); it!=endPoints->end(); it++)	{
		 r = (*it)->getY();
		 c = (*it)->getX();
		 //check if point is not already connected (=still set in edgeImage)
		 if (edgeImage->at<uchar>(r, c)>0) {
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
				} while (!getNextPoint(&r, &c, edgeImage));
				segments->push_back(es);//end of edge reached; segment complete
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
	if (d != 0 || (a2 - a1) < 0) {
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
	r7 = &(*P - *L1);
	r8 = &(*P - *R1);
	r9 = &(*R2 - *R1);

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
#ifdef DEBUB_CURVE_SEG
	std::fstream csf;
	csf.open(CURVE_SEG_DEBUG, std::ios::out);
#endif 

	//check every edge segment
	int nCurvSegs = 0;
	for (std::list<EdgeSegment*>::const_iterator i = edgeSegs->begin(); i!=edgeSegs->end() ; i++) {
		nCurvSegs+=(*i)->curveSegmentation(curveSegs,&csf);
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
void connectSegments(EdgeSegment* cS1, EdgeSegment* cS2,std::set<EllipticalArc*> *arcs) {
	EllipticalArc* arcS1=NULL,*arcS2=NULL;
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
		arcS1->addSegment(cS1);
		arcS1->addSegment(cS2);
		arcs->insert(arcS1);
	}
	else if (arcS1 != NULL && arcS2 != NULL && arcS1 != arcS2) {
		//both segments are already a part of different arcs, so join them
		arcS1->joinArcs(arcS2);
		arcs->erase(arcS2);
		delete(arcS2);
	}
	else if (arcS1 != NULL) {
		//only segment cS1 is part of an arc, add cS2
		arcS1->addSegment(cS2);
	}
	else if (arcS2 != NULL) {
		arcS2->addSegment(cS1);
	}
}

int curveGrouping(std::list<EdgeSegment*> *curveSegs, std::set<EllipticalArc*> *arcs) {
#ifdef DEBUB_CURVE_GRP
	std::fstream csf;
	csf.open(CURVE_GRP_DEBUG, std::ios::out);
#endif 
	Point *nfirst = NULL, *nend = NULL, *mfirst = NULL, *mend = NULL, *N1 = NULL, *M1 = NULL, *N2 = NULL, *M2 = NULL, *r1 = NULL, *r2 = NULL;
	Point *L1 = NULL, *L2 = NULL, *R1 = NULL, *R2 = NULL, *P = NULL;
	EdgeSegment *cS_min=NULL;
	int order_min; //eS_min must be added in this order
	int order_tmp;
	int d;
	int mEnE, mEnB, mBnE, mBnB;
	double a_min = 2*PI,a_tmp;
	//neightbourhood curve grouping:
	//search for every m-th curve segment the n-th curve segment that has the min difference of tangents at their end points
	for (std::list<EdgeSegment*>::iterator n = curveSegs->begin(); n != curveSegs->end(); n++) {
		if (*n != NULL) {
			EllipticalArc *tmp = new EllipticalArc;
			tmp->addSegment((*n));
			arcs->insert(tmp);
#ifdef DEBUB_CURVE_GRP
			csf << "Seg " << (*n)->ID << std::endl;
#endif
			nfirst = (*n)->getFirstPoint();
			nend = (*n)->getLastPoint();
			for (std::list<EdgeSegment*>::iterator m = curveSegs->begin(); m != curveSegs->end(); m++) {
				if (*n != *m) {
					mfirst = (*m)->getFirstPoint();
					mend = (*m)->getLastPoint();
					//calc distance between n and m
					mEnE = (*mend - *nend).norm()+.5;
					mEnB = (*mend - *nfirst).norm()+.5;
					mBnE = (*mfirst - *nend).norm()+.5;
					mBnB = (*mfirst - *nfirst).norm()+.5;
					d = std::min(std::min(mEnE, mEnB), std::min(mBnE, mBnB));
					if (d>0 && d < D0) {
						if (d == mEnE) {
							order_tmp = END_END;
							M1 = (*m)->getLastPoint();
							N1 = (*n)->getLastPoint();
							M2 = (*m)->getNextToLastPoint();
							N2 = (*n)->getNextToLastPoint();
						}
						else if (d == mEnB) {
							order_tmp = BEGIN_END;
							M1 = (*m)->getLastPoint();
							N1 = (*n)->getFirstPoint();
							M2 = (*m)->getNextToLastPoint();
							N2 = (*n)->getSecondPoint();
						}
						else if (d == mBnE) {
							order_tmp = END_BEGIN;
							M1 = (*m)->getFirstPoint();
							N1 = (*n)->getLastPoint();
							M2 = (*m)->getSecondPoint();
							N2 = (*n)->getNextToLastPoint();
						}
						else if (d == mBnB) {
							order_tmp = BEGIN_BEGIN;
							M1 = (*m)->getFirstPoint();
							N1 = (*n)->getFirstPoint();
							M2 = (*m)->getSecondPoint();
							N2 = (*n)->getSecondPoint();
						}

						//calc angle between the tangents of m and n
						r1 = &(*M1 - *M2);
						r2 = &(*N2 - *N1);
						a_tmp = acos((*r1 * *r2) / (r1->norm()* r2->norm()));
#ifdef DEBUB_CURVE_GRP
						csf << "Abstand: "<<d<<"  Winkel an Enden: M1(" << M1->getX() << "," << M1->getY() << ") M2(" << M2->getX() << "," << M2->getY() << ") und N1(" << N1->getX() << "," << N1->getY() << ") N2(" << N2->getX() << "," << N2->getY() <<") ::"<<a_tmp<<std::endl ;
#endif
						if (a_tmp < a_min) {
							//current angle between n and m is the smallest so far, so keep it in mind
							order_min = order_tmp;
							a_min = a_tmp;
							cS_min = (*m);
						}
					}
				}
			}

			//group n with the curve segment which has the smallest angle between tangents
			if (cS_min != NULL) {
				//TODO: an dieser Stelle sollte curvature condi getesten werden, damit nicht zuvor getrennt Segmente erneut verknuepft werden!!!!
				//test curvature cond
				if (!curvatureCond(M2, M1, N1, N2) && !curvatureCond(N2, N1, M1, M2)) {
					connectSegments((*n), cS_min, arcs);
#ifdef DEBUB_CURVE_GRP
					csf << "Seg " << (*n)->ID << " mit Seg " << cS_min->ID << "-> kleinster Winkel: " << a_min << std::endl;
					csf << "################ Zusammengefuegt ######################" << std::endl;
#endif
				}
			}
			cS_min = NULL;
			a_min = 2 * PI;
		}

	}

	//global curve grouping:
	for (std::set<EllipticalArc*>::iterator n = arcs->begin(); n != arcs->end(); n++) {
		if (*n != NULL) {
			for (std::list<EdgeSegment*>::iterator m = curveSegs->begin(); m != curveSegs->end(); m++) {

			}
		}
	}

#ifdef DEBUB_CURVE_GRP
	csf.close();
#endif

	return arcs->size();
}