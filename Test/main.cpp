#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Defines.h"
#include "EdgeSegment.h"
#include "Point.h"
#include "PreProcessing.hpp"


using namespace std;

//Mat src; Mat src_gray;
//int thresh = 100;
//int max_thresh = 255;
//RNG rng(12345);

/// Function header
void thresh_callback(int, void*);

/** @function main */
int main(int argc, char** argv) {
	/*/// Load source image and convert it to gray
	src = imread("D:\\Users\\Boschki\\Documents\\Visual Studio 2013\\Projects\\Test\\strassenschilder.jpg");

	/// Convert image to gray and blur it
	cvtColor(src, src_gray, CV_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));

	/// Create Window
	char* source_window = "Source";
	namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	imshow(source_window, src);

	createTrackbar(" Canny thresh:", "Source", &thresh, max_thresh, thresh_callback);
	thresh_callback(0, 0);

	waitKey(0);*/

	/// Load source image and convert it to gray
	
	cv::Mat src = cv::imread("..\\strassenschilder2.png", CV_LOAD_IMAGE_GRAYSCALE);
	//cv::Mat src = cv::imread("..\\t.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	//cv::Mat src = cv::imread("..\\strassenschilder.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat test = cv::Mat::zeros(20, 20, CV_8UC1);
	cv::RNG rng(12353);
	
	test.at<uchar>(0, 0) = 255;/*
	test.at<uchar>(5, 6) = 255;
	test.at<uchar>(5, 7) = 255;*/
	//src = test;
	cv::Mat edgeImage = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
	//cv::Mat edgeImage2(src);
	cv::Mat segImage = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
	cv::Mat lineSegmentedEdges = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
	cv::Mat endPointImage = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

	list<EdgeSegment*> edgeSegments;
	list<Point*> endPoints;

	/// Create Window
	printf("Hallo Startrows: %d, cols: %d\n",src.rows,src.cols);
	char* source_window = "Source";
	cv::namedWindow(source_window, CV_WINDOW_AUTOSIZE);
	cv::imshow(source_window, src);

	//detect edges
	edgeDetection(src, edgeImage, 100, 150, 3);
	char* edge_window = "Edges";
	cv::namedWindow(edge_window, CV_WINDOW_AUTOSIZE);
	cv::imshow(edge_window, edgeImage);
	cv::imwrite("..\\edgeImage.jpg", edgeImage);
	cv::Mat edgeImage2(edgeImage);


	//find ends
	int nEnds  = findEnds(&endPoints, edgeImage2);
	char* edge2_window = "Edges2";
	cv::namedWindow(edge2_window, CV_WINDOW_AUTOSIZE);
	cv::imshow(edge2_window, edgeImage2);
	cv::imwrite("..\\edgeImage_NachFindEnds.jpg", edgeImage2);


	//link edges
	int nSegs = 0;
	nSegs = edgeLinking(edgeImage2, &endPoints, &edgeSegments);
	printf("Ends: %d\nSegments: %d", nEnds,nSegs);

	//show segments
	char* segment_window = "Segments";
	cv::namedWindow(segment_window, CV_WINDOW_AUTOSIZE);

	for (list<EdgeSegment*>::iterator it = edgeSegments.begin(); it != edgeSegments.end(); it++){
		cv::Vec3b color(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		if ((*it)->getLength() > MIN_LENGTH) {
			(*it)->drawToImage(&segImage, color);
		}
	}
	cv::imshow(segment_window, segImage);
	cv::imwrite("..\\Segments.jpg", segImage);
	
	//line segmentation for each Edge segment
	list<EdgeSegment*>::iterator it;
	for (it=edgeSegments.begin(); it!=edgeSegments.end();)	{
		if ((*it)->getLength() > MIN_LENGTH) {
			(*it)->lineSegmentation(D_TOL);
			it++;
		}else{
			it=edgeSegments.erase(it);
		}
	}

#ifdef DEBUG_SHOW_LINESEGMENTEDEDGES
	//draw segments
	for (list<EdgeSegment*>::iterator it = edgeSegments.begin(); it != edgeSegments.end(); it++){
		cv::Vec3b color(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		(*it)->drawToImage(&lineSegmentedEdges, color);
		

		//cv::waitKey(0);
	}
	char* lineSegmentedEdges_window = "LineSegmentedEdges";
	cv::namedWindow(lineSegmentedEdges_window, CV_WINDOW_AUTOSIZE);
	cv::imshow(lineSegmentedEdges_window, lineSegmentedEdges);
	cv::imwrite("..\\lineSegmentedEdges.jpg", lineSegmentedEdges);
#endif

	//curve segmentation
#ifdef DEBUB_CURVE_SEG
	//copy each edge Segment
	list<EdgeSegment*> copy_segs;
	for (list<EdgeSegment*>::const_iterator i = edgeSegments.begin(); i!=edgeSegments.end(); i++)	{
		EdgeSegment *es=new EdgeSegment((**i));
		copy_segs.push_back(es);
	}
	int b = 1;
#endif
	cv::waitKey(0);
	return(0);
}


/** @function thresh_callback */
//void thresh_callback(int, void*)
//{
//	Mat canny_output;
//	vector<vector<Point> > contours;
//	vector<Vec4i> hierarchy;
//
//	/// Detect edges using canny
//	Canny(src_gray, canny_output, thresh, thresh * 2, 3);
//	/// Find contours
//	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
//
//	/// Draw contours
//	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
//	for (int i = 0; i< contours.size(); i++)
//	{
//		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
//		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
//	}
//
//	/// Show in a window
//	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
//	imshow("Contours", drawing);
//}