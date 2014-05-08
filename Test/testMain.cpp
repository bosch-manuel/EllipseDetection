//#include "Point.h"
//#include "Defines.h"
//#include "EdgeSegment.h"
//#include "EllipticalArc.h"
//#include <math.h>
//#include <stdio.h>
//#include <iostream>
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
////
////using namespace std;
////int main(int argc, char** argv) {
////	Point P(1,1);
////	Point L2(74,430);
////	Point L1(75,421);
////	Point R1(75,416);
////	Point R2(69,406);
////
////	Point r0 = (R1 - R2);
////	Point r1 = (L2 - L1);
////	double a1 = acos((r0 * r1) / (r0.norm()* r1.norm()))*(180 / PI);
////	
////	EdgeSegment e1;
////	EdgeSegment e2;
////	EdgeSegment e3;
////
////	EllipticalArc arc;
////	arc.addSegment(&e2);
////	arc.addSegment(&e1);
////	bool b = arc.containsSegment(&e2);
////	bool c = arc.containsSegment(&e3);
////	getchar();
////
////}
////#include "opencv2/imgproc/imgproc.hpp"
////#include "opencv2/highgui/highgui.hpp"
////#include <stdlib.h>
////#include <stdio.h>
//
////using namespace cv;
//
///// Global variables
////
////Mat src, src_gray;
////Mat dst, detected_edges;
////
////int edgeThresh = 1;
////int lowThreshold;
////int const max_lowThreshold = 100;
////int ratio = 3;
////int kernel_size = 3;
////char* window_name = "Edge Map";
//
///**
//* @function CannyThreshold
//* @brief Trackbar callback - Canny thresholds input with a ratio 1:3
//*/
////void CannyThreshold(int, void*)
////{
////	/// Reduce noise with a kernel 3x3
////	blur(src_gray, detected_edges, Size(3, 3));
////
////	/// Canny detector
////	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);
////
////	/// Using Canny's output as a mask, we display our result
////	dst = Scalar::all(0);
////
////	src.copyTo(dst, detected_edges);
////	imshow(window_name, dst);
////}
//
//
///** @function main */
//int main(int argc, char** argv)
//{
//	///// Load an image
//	//src_gray =  cv::imread("..\\Test.png", CV_LOAD_IMAGE_GRAYSCALE);
//
//	//
//
//	///// Create a matrix of the same type and size as src (for dst)
//	//dst.create(src.size(), src.type());
//
//	///// Convert the image to grayscale
//	////cvtColor(src, src_gray, CV_BGR2GRAY);
//
//	///// Create a window
//	//namedWindow(window_name, CV_WINDOW_AUTOSIZE);
//
//	///// Create a Trackbar for user to enter threshold
//	//createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
//
//	///// Show the image
//	//CannyThreshold(0, 0);
//
//	///// Wait until user exit program by pressing a key
//	//waitKey(0);
//
//	Point *p1 = new Point(1, 1);
//	Point *p2 = new Point(2, 2);
//	Point *p3 = new Point(3, 3);
//	Point *p4 = new Point(4, 4);
//
//	EdgeSegment e1;
//	e1.push_backPoint(p1);
//	e1.push_backPoint(p2);
//	e1.push_backPoint(p3);
//	e1.push_backPoint(p4);
//
//	std::list<Point*>::const_iterator cb = e1.cbegin();
//	std::list<Point*>::const_iterator ce = e1.cend();
//
//	for (; cb!=ce; cb++){
//		std::cout << " " << (*cb)->getX()<<std::endl;
//	}
//
//	std::list<Point*>::const_reverse_iterator crb = e1.crbegin();
//	std::list<Point*>::const_reverse_iterator cre = e1.crend();
//	for (; crb!=cre; crb++)	{
//		std::cout << " " << (*crb)->getX() << std::endl;
//	}
//	while (1){}
//	return 0;
//}