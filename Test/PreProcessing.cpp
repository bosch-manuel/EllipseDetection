#include "PreProcessing.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "Point.h"
#include <list>
#include <fstream>

#define DEBUG
#define EDGE_DEBUG "..\\EdgeLink_Debug.txt"
#define FINDENDS_DEBUG "..\\FindEnds_Debug.txt"
#ifdef DEBUG
std::fstream f;
//char *log = "..\\PreProcessing_Debug.txt";
#endif
int edgeDetection(cv::Mat input_image, cv::Mat edge_image, int lowThreshold, int highThreshold, int kernelSize) {
	cv::Canny(input_image, edge_image, lowThreshold, highThreshold, kernelSize);

	return 0;
}

int checkNeighbors(int r, int c,cv::Mat edgeImage) {
	int r1, r2, r3, c1, c2, c3, crossings,rows,cols;
	// | 1 | 8 | 7 |
	//	--- --- ---
	// | 2 | x | 6 |
	//  --- --- ---
	// | 3 | 4 | 5 |
	uchar a[8] = { 0 };
	uchar b[8] = {0};
	crossings = 0;
	rows = edgeImage.rows;
	cols = edgeImage.cols;
	//check out the neighborhood
	r1 = r - 1; r2 = r; r3 = r + 1;
	c1 = c - 1; c2 = c; c3 = c + 1;
	//first row
	if (r1>-1) {
		if ((c1 > -1) && edgeImage.at<uchar>(r1, c1)>0){
			a[0] = 1;
			b[7] = 1;
		}
		if (edgeImage.at<uchar>(r1, c2)>0){
			a[7] = 1;
			b[6] = 1;
		}
		if ((c3 < cols) && edgeImage.at<uchar>(r1, c3)>0){
			a[6] = 1;
			b[5] = 1;
		}
	}

	if ((c1 > -1) && edgeImage.at<uchar>(r, c1)>0){
		a[1] = 1;
		b[0] = 1;
	}
	if ((c3 < cols) && edgeImage.at<uchar>(r, c3)>0){
		a[5] = 1;
		b[4] = 1;
	}

	if (r3<rows) {
		if ((c1 > -1) && edgeImage.at<uchar>(r3, c1)>0){
			a[2] = 1;
			b[1] = 1;
		}
		if (edgeImage.at<uchar>(r3, c2)>0){
			a[3] = 1;
			b[2] = 1;
		}
		if ((c3 < cols) && edgeImage.at<uchar>(r3, c3)>0){
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
		if ((c1 > -1) && edgeImage.at<uchar>(r1, c1)>0){
			endPoints->push_back(new Point(r1, c1));
			//endPointsImage.at<uchar>(r1, c1)=value;
			ends++;
		}
		if (edgeImage.at<uchar>(r1, c2)>0){
			endPoints->push_back(new Point(r1, c2));
			//endPointsImage.at<uchar>(r1, c2) = value;
			ends++;
		}
		if ((c3 < cols) && edgeImage.at<uchar>(r1, c3)>0){
			endPoints->push_back(new Point(r1, c3));
			//endPointsImage.at<uchar>(r1, c3) = value;
			ends++;
		}
	}

	if ((c1 > -1) && edgeImage.at<uchar>(r, c1)>0){
		endPoints->push_back(new Point(r, c1));
		//endPointsImage.at<uchar>(r, c1) = value;
		ends++;
	}
	//if ((c3 < cols) && edgeImage.at<uchar>(r, c3)>0){
	//	endPoints->push_back(new Point(r, c3));
	//	//endPointsImage.at<uchar>(r, c3) = value;
	//	ends++;
	//}

	//if (r3<rows) {
	//	if ((c1 > -1) && edgeImage.at<uchar>(r3, c1)>0){
	//		endPoints->push_back(new Point(r3, c1));
	//		//endPointsImage.at<uchar>(r3, c1) = value;
	//		ends++;
	//	}
	//	if (edgeImage.at<uchar>(r3, c2)>0){
	//		endPoints->push_back(new Point(r3, c2));
	//		//endPointsImage.at<uchar>(r3, c2) = value; 
	//		ends++;
	//	}
	//	if ((c3 < cols) && edgeImage.at<uchar>(r3, c3)>0){
	//		endPoints->push_back(new Point(r3, c3));
	//		//endPointsImage.at<uchar>(r3, c3) = value;
	//		ends++;
	//	}
	//}
	return ends;
}

int findEnds(std::list<Point*> *endPoints, cv::Mat edgeImage) {
#ifdef DEBUG
	f.open(FINDENDS_DEBUG,std::ios::out);
#endif

	int value = 255;
	int rows, cols;
	rows = edgeImage.rows;
	cols = edgeImage.cols;
	int crossings = 0;
	int ends=0;
	int junctions = 0;

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			//(r,c) is an end point if within his 3x3 neighborhood exactly 2 pixels are set
			if (edgeImage.at<uchar>(r, c) > 0) {
				crossings = checkNeighbors(r, c, edgeImage);
#ifdef DEBUG
				//printf("Gesetzt in Nachbarschaft von (%d,%d): %d \n",r,c, nSet);
				f << "Crossings in Nachbarschaft von" << r << "  " << c << "  " << crossings << std::endl;
#endif
				if (crossings == 0) {//isolated pixel, remove it
					edgeImage.at<uchar>(r, c) = 0;
#ifdef DEBUG
					//printf("Isoliertes Pixel\n");
					f << "Isoliertes Pixel" << std::endl;
#endif
				}else if (crossings == 2) {//end point found, save it
					//endPointsImage.at<uchar>(r, c)=value;
					endPoints->push_back(new Point(r, c));
					ends++;
				}else if (crossings > 5) {//junction, remove it and add adjacent pixels as endpoints
					edgeImage.at<uchar>(r, c) = 0;
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
	f << "############"<<std::endl<< "Ends: "<<ends<<" Juctions: "<<junctions << std::endl;
	f.close();	
#endif

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
		 if (edgeImage.at<uchar>(r,c)>0) {
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
				 edgeImage.at<uchar>(r, c) = 0; //delet current point
			 } while (!getNextPoint(&r, &c, edgeImage));
			 segments->push_back(es);//end of edge reached; segment complete
		 }

	}

#ifdef DEBUG
	f <<std::endl<< "Alle Endpunkte zugeordnet, Suche nach isolierten loops" << std::endl;
#endif

	//second step: search for any unattached edge points and connect them to segments. These correspond to isolated loops
	for (int row = 0; row < edgeImage.rows; row++){
		for (int col=0; col < edgeImage.cols; col++) {
			//check if point is and edge point
			if (edgeImage.at<uchar>(row, col)>0) {
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
					edgeImage.at<uchar>(r, c) = 0; //delet current point
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