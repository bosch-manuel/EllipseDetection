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

int removeJunction(int r, int c, cv::Mat edgeImage, cv::Mat endPoints) {
	int r1, r2, r3, c1, c2, c3, nSet, rows, cols,ends;
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
			endPoints.at<uchar>(r1, c1)=value;
			ends++;
		}
		if (edgeImage.at<uchar>(r1, c2)){
			endPoints.at<uchar>(r1, c2) = value;
			ends++;
		}
		if ((c3 < cols) && edgeImage.at<uchar>(r1, c3)){
			endPoints.at<uchar>(r1, c3) = value;
			ends++;
		}
	}

	if ((c1 > -1) && edgeImage.at<uchar>(r, c1)){
		endPoints.at<uchar>(r, c1) = value;
		ends++;
	}
	if ((c3 < cols) && edgeImage.at<uchar>(r, c3)){
		endPoints.at<uchar>(r, c3) = value;
		ends++;
	}

	if (r3<rows) {
		if ((c1 > -1) && edgeImage.at<uchar>(r3, c1)){
			endPoints.at<uchar>(r3, c1) = value;
			ends++;
		}
		if (edgeImage.at<uchar>(r3, c2)){
			endPoints.at<uchar>(r3, c2) = value; ends++;
		}
		if ((c3 < cols) && edgeImage.at<uchar>(r3, c3)){
			endPoints.at<uchar>(r3, c3) = value;
			ends++;
		}
	}
	return ends;
}

int findEndsJunctions(cv::Mat endPoints, cv::Mat edgeImage) {
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
					endPoints.at<uchar>(r, c)=value;
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

int edgeSegmentation(cv::Mat edge_image, std::list<EdgeSegment> *segments) {
	for (int i = 0; i < segments->size(); i++) {

	}
	return 0;
}