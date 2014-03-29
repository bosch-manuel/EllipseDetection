#include "PreProcessing.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "Point.h"
#include <list>

int edgeDetection(cv::Mat input_image, cv::Mat edge_image, int lowThreshold, int highThreshold, int kernelSize) {
	cv::Canny(input_image, edge_image, lowThreshold, highThreshold, kernelSize);

	return 0;
}

int checkNeighbors(int r, int c,cv::Mat edgeImage) {
	int r1, r2, r3, c1, c2, c3, nSet,rows,cols;
	nSet = 1;
	rows = edgeImage.rows;
	cols = edgeImage.cols;
	//check out the neighborhood
	r1 = r - 1; r2 = r; r3 = r + 1;
	c1 = c - 1; c2 = c; c3 = c + 1;
	//first row
	if (r1>-1) {
		if ((c1 > -1) && edgeImage.at<uchar>(r1, c1)){
			nSet++;
		}
		if (edgeImage.at<uchar>(r1, c2)){
			nSet++;
		}
		if ((c3 < cols) && edgeImage.at<uchar>(r1, c3)){
			nSet++;
		}
	}

	if ((c1 > -1) && edgeImage.at<uchar>(r, c1)){
		nSet++;
	}
	if ((c3 < cols) && edgeImage.at<uchar>(r, c3)){
		nSet++;
	}

	if (r3<rows) {
		if ((c1 > -1) && edgeImage.at<uchar>(r3, c1)){
			nSet++;
		}
		if (edgeImage.at<uchar>(r3, c2)){
			nSet++;
		}
		if ((c3 < cols) && edgeImage.at<uchar>(r3, c3)){
			nSet++;
		}
	}

	return nSet;
}

int removeJunction(int r, int c, cv::Mat edgeImage, std::list<Point*> *endPoints) {
	int r1, r2, r3, c1, c2, c3, rows, cols,ends;
	int value = 255;
	ends = 0;
	rows = edgeImage.rows;
	cols = edgeImage.cols;
	//check out the neighborhood
	r1 = r - 1; r2 = r; r3 = r + 1;
	c1 = c - 1; c2 = c; c3 = c + 1;
	//first row
	if (r1>-1) {
		if ((c1 > -1) && edgeImage.at<uchar>(r1, c1)){
			endPoints->push_back(new Point(r1, c1));
			//endPointsImage.at<uchar>(r1, c1)=value;
			ends++;
		}
		if (edgeImage.at<uchar>(r1, c2)){
			endPoints->push_back(new Point(r1, c2));
			//endPointsImage.at<uchar>(r1, c2) = value;
			ends++;
		}
		if ((c3 < cols) && edgeImage.at<uchar>(r1, c3)){
			endPoints->push_back(new Point(r1, c3));
			//endPointsImage.at<uchar>(r1, c3) = value;
			ends++;
		}
	}

	if ((c1 > -1) && edgeImage.at<uchar>(r, c1)){
		endPoints->push_back(new Point(r, c1));
		//endPointsImage.at<uchar>(r, c1) = value;
		ends++;
	}
	if ((c3 < cols) && edgeImage.at<uchar>(r, c3)){
		endPoints->push_back(new Point(r, c3));
		//endPointsImage.at<uchar>(r, c3) = value;
		ends++;
	}

	if (r3<rows) {
		if ((c1 > -1) && edgeImage.at<uchar>(r3, c1)){
			endPoints->push_back(new Point(r3, c1));
			//endPointsImage.at<uchar>(r3, c1) = value;
			ends++;
		}
		if (edgeImage.at<uchar>(r3, c2)){
			endPoints->push_back(new Point(r3, c2));
			//endPointsImage.at<uchar>(r3, c2) = value; 
			ends++;
		}
		if ((c3 < cols) && edgeImage.at<uchar>(r3, c3)){
			endPoints->push_back(new Point(r3, c3));
			//endPointsImage.at<uchar>(r3, c3) = value;
			ends++;
		}
	}
	return ends;
}

int findEnds(std::list<Point*> *endPoints, cv::Mat edgeImage) {
	int value = 255;
	int rows, cols;
	rows = edgeImage.rows;
	cols = edgeImage.cols;
	int nSet = 1;
	int ends=0;

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			//(r,c) is an end point if within his 3x3 neighborhood exactly 2 pixels are set
			if (edgeImage.at<uchar>(r, c) > 0) {
				nSet= checkNeighbors( r,  c,edgeImage);
				/*for (int rn = r-1; rn <= rn+1; rn++)	{
					for (int cn = c-1; cn <= cn+1; cn++)	{
						if ((-1 < rn) && (rn < rows) && (-1 < cn) && (cn < cols)) {
							if (edgeImage.at<uchar>(rn, cn) > 0) {
								nSet++;
							}
						}
					}
				}*/

				if (nSet == 1) {//isolated pixel, remove it
					edgeImage.at<uchar>(r, c) = 0;
				}
				else if (nSet == 2) {//end point found, save it
					//endPointsImage.at<uchar>(r, c)=value;
					endPoints->push_back(new Point(r, c));
					ends++;
				}
				else if (nSet > 3) {//junction, remove it and add adjacent pixels as endpoints
					edgeImage.at<uchar>(r, c) = 0;
					/*for (int rn = r - 1; rn <= rn + 1; rn++)	{
						for (int cn = c - 1; cn <= cn + 1; cn++)	{
							if ((-1 < rn) && (rn < rows) && (-1 < cn) && (cn < cols)) {
								if (edgeImage.at<uchar>(rn, cn) > 0) {
									endPoints.at<uchar>(r, c);
									ends++;
								}
							}
						}
					}*/
					ends += removeJunction( r,  c, edgeImage, endPoints);
				}
				nSet = 1;
			}
		}
	}
	return ends;
}
/*Searches the next edge point to Point(row,col)
return	0 if a point was found, r and c contain the coordinates of the next point;
		-1 when no point was found*/
int getNextPoint(int *row, int *col,cv::Mat edgeImage) {
	int r,c,r1, r2, r3, c1, c2, c3, ret, rows, cols;
	ret = -1;
	r = *row;
	c = *col;
	rows = edgeImage.rows;
	cols = edgeImage.cols;
	//check out the neighborhood
	r1 = r - 1; r2 = r; r3 = r + 1;
	c1 = c - 1; c2 = c; c3 = c + 1;
	//first row
	if (r1>-1) {
		if ((c1 > -1) && edgeImage.at<uchar>(r1, c1)){
			ret=0;
			*row = r1;
			*col = c1;
			return 0;
		}else if (edgeImage.at<uchar>(r1, c2)){
			ret = 0;
			*row = r1;
			*col = c2;
			return 0;
		}else if ((c3 < cols) && edgeImage.at<uchar>(r1, c3)){
			ret = 0;
			*row = r1;
			*col = c3;
			return 0;
		}
	}else if ((c1 > -1) && edgeImage.at<uchar>(r, c1)){
		ret = 0;
		*row = r;
		*col = c1;
		return 0;
	}else if ((c3 < cols) && edgeImage.at<uchar>(r, c3)){
		ret = 0;
		*row = r;
		*col = c3;
		return 0;
	}else if (r3<rows) {
		if ((c1 > -1) && edgeImage.at<uchar>(r3, c1)){
			ret = 0;
			*row = r3;
			*col = c1;
			return 0;
		}else if (edgeImage.at<uchar>(r3, c2)){
			ret = 0;
			*row = r3;
			*col = c2;
			return 0;
		}else if ((c3 < cols) && edgeImage.at<uchar>(r3, c3)){
			ret = 0;
			*row = r3;
			*col = c3;
			return 0;
		}
	}

	return ret;
}

int edgeLinking(cv::Mat edgeImage, std::list<Point*> *endPoints, std::list<EdgeSegment*> *segments) {
	//first step: connect all known end points to edge segments
	std::list<Point*>::iterator it;
	int r, c, nSegs;
	EdgeSegment *es;
	nSegs = 0;
	for (it=endPoints->begin(); it!=endPoints->end(); it++)	{
		 r = (*it)->getY();
		 c = (*it)->getX();
		 //check if point is not already connected (=still set in edgeImage)
		 if (edgeImage.at<uchar>(r,c)>0) {
			 es = new EdgeSegment();
			 nSegs++;
			 //trace until the end of the edge
			 do {
				 es->push_backPoint(new Point(r, c));//add point to current segment
				 edgeImage.at<uchar>(r, c) = 0; //delet current point
			 } while (!getNextPoint(&r, &c, edgeImage));
			 segments->push_back(es);//end of edge reached; segment complete
		 }

	}
	//second step: search for any unattached edge points and connect them to segments. These correspond to isolated loops
	for (int row = 0; row < edgeImage.rows; row++){
		for (int col=0; col < edgeImage.cols; col++) {
			//check if point is and edge point
			if (edgeImage.at<uchar>(row, col)>0) {
				es = new EdgeSegment();
				nSegs++;
				r = row;
				c = col;
				//trace until the end of the edge
				do {
					es->push_backPoint(new Point(r, c));//add point to current segment
					edgeImage.at<uchar>(r, c) = 0; //delet current point
				} while (!getNextPoint(&r, &c, edgeImage));
				segments->push_back(es);//end of edge reached; segment complete
			}
		}
	}
	return nSegs;
}