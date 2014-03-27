#include "PreProcessing.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "Point.h"
#include <list>

int edgeDetection(cv::Mat input_image, cv::Mat edge_image, int lowThreshold, int highThreshold, int kernelSize) {
	cv::Canny(input_image, edge_image, lowThreshold, highThreshold, kernelSize);

	return 0;
}

int findEndsJunctions(std::list<Point*> *endPoints, cv::Mat edgeImage) {
	int rows, cols;
	rows = edgeImage.rows;
	cols = edgeImage.cols;
	int nSet = 0;
	int ends=0;

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			//(r,c) is an end point if within his 3x3 neighborhood exactly 2 pixels are set
			if (edgeImage.at<uchar>(r, c) > 0) {
				//check out the neighborhood
				for (int rn = r-1; rn <= rn+1; rn++)	{
					for (int cn = c-1; cn <= cn+1; cn++)	{
						if ((-1 < rn) && (rn < rows) && (-1 < cn) && (cn < cols)) {
							if (edgeImage.at<uchar>(rn, cn) > 0) {
								nSet++;
							}
						}
					}
				}

				if (nSet == 1) {//isolated pixel, remove it
					//edgeImage.at<uchar>(r, c) = 0;
				}
				else if (nSet == 2) {//end point found, save it
					//endPoints->push_back(new Point(r, c));
					ends++;
				}
				else if (nSet > 3) {//junction, remove it and add adjacent pixels as endpoints
					//edgeImage.at<uchar>(r, c) = 0;
					for (int rn = r - 1; rn <= rn + 1; rn++)	{
						for (int cn = c - 1; cn <= cn + 1; cn++)	{
							if ((-1 < rn) && (rn < rows) && (-1 < cn) && (cn < cols)) {
								if (edgeImage.at<uchar>(rn, cn) > 0) {
								//	endPoints->push_back(new Point(rn, cn));
									ends++;
								}
							}
						}
					}
				}
				nSet = 0;
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