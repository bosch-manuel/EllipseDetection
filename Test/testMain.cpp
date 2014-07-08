//#include "Point.h"
//#include "Defines.h"
//#include "EdgeSegment.h"
//#include "EllipticalArc.h"
//#include <math.h>
//#include <stdio.h>
//#include <iostream>
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "Ellipse.h"
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
//#include "PreProcessing.hpp"
///** @function main */
//int main(int argc, char** argv)
//{
//	/////// Load an image
//	////src_gray =  cv::imread("..\\Test.png", CV_LOAD_IMAGE_GRAYSCALE);
//
//	////
//
//	/////// Create a matrix of the same type and size as src (for dst)
//	////dst.create(src.size(), src.type());
//
//	/////// Convert the image to grayscale
//	//////cvtColor(src, src_gray, CV_BGR2GRAY);
//
//	/////// Create a window
//	////namedWindow(window_name, CV_WINDOW_AUTOSIZE);
//
//	/////// Create a Trackbar for user to enter threshold
//	////createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
//
//	/////// Show the image
//	////CannyThreshold(0, 0);
//
//	/////// Wait until user exit program by pressing a key
//	////waitKey(0);
//	//cv::Mat img(100, 50, CV_8UC3);
//	//img= cv::Scalar(255, 255, 255);
//	Point *p1 = new Point(62.70561, 148.90781);
//	Point *p2 = new Point(68.10255, 154.88224);
//	Point *p3 = new Point(75.8279, 158.48876);
//	Point *p4 = new Point(81.7896, 159.40347);
//	Point *p5 = new Point(88.72887, 158.76773);
//
//	Point *m1 = new Point(65.4642339242877, 140.079099341533);
//	Point *m2 = new Point(73.9003151634729, 149.854328696838);
//	Point *m3 = new Point(84.7094752824861, 153.164115197268);
//	Point *m4 = new Point(94.9257364835308, 151.135928564277);
//	Point *m5 = new Point(100.750128013752, 147.163624121972);
//
//	Point *o1 = new Point(64.6990540925175, 133.817673081278);
//	Point *o2 = new Point(72.6577372602929, 128.840132905923);
//	Point *o3 = new Point(82.1104386448367, 127.583485036185);
//	Point *o4 = new Point(90.0629739353950, 129.214062735247);
//	Point *o5 = new Point(98.6474787423100, 134.870137816917);
//
//	Point *n1 = new Point(74.8731296372179, 113.721914829758);
//	Point *n2 = new Point(84.9337457310506, 111.097542843625);
//	Point *n3 = new Point(96.8343027881680, 114.284466956049);
//	Point *n4 = new Point(103.333183736084, 120.321501450820);
//	Point *n5 = new Point(106.586722995817, 127.503170525708);
//
//	std::vector<Point*> p, m, o, n;
//
//	p.push_back(p1);
//	p.push_back(p2);
//	p.push_back(p3);
//	p.push_back(p4);
//	p.push_back(p5);
//
//	m.push_back(m1);
//	m.push_back(m2);
//	m.push_back(m3);
//	m.push_back(m4);
//	m.push_back(m5);
//
//	n.push_back(n1);
//	n.push_back(n2);
//	n.push_back(n3);
//	n.push_back(n4);
//	n.push_back(n5);
//
//	o.push_back(o1);
//	o.push_back(o2);
//	o.push_back(o3);
//	o.push_back(o4);
//	o.push_back(o5);
//
//	Ellipse *e = calcEllipse(&p);
//
//	Ellipse *e = calcEllipse(&m);
//
//	Ellipse *e = calcEllipse(&n);
//
//	Ellipse *e = calcEllipse(&o);
//
//
//	//std::list<EdgeSegment*> segs;
//	//std::list<Ellipse*> els;
//	//EdgeSegment *e1 = new EdgeSegment(), *e2 = new EdgeSegment();
//	//
//	//e1->push_backPoint(p1);
//	//e1->push_backPoint(p2);
//	//e1->push_backPoint(p3);
//	//e1->push_backPoint(p4);
//	//e1->push_backPoint(p5);
//	//e1->push_backPoint(p6);
//	//e1->push_backPoint(p7);
//	//e1->push_backPoint(p8);
//
//	//e2->push_backPoint(p1);
//	//e2->push_backPoint(p2);
//	//e2->push_backPoint(p3);
//	//e2->push_backPoint(p4);
//	//e2->push_backPoint(p5);
//	//e2->push_backPoint(p6);
//	//e2->push_backPoint(p7);
//	//e2->push_backPoint(p8);
//	//Ellipse *e;
//
//	//segs.push_back(e1);
//	//segs.push_back(e2);
//	//fitEllipses(&segs,&els);
//
//	//calc
//
//	//int *d = new int[segs.size()];
//	//for (size_t i = 0; i < segs.size(); i++)
//	//{
//
//	//}
//	///*e=e1.calcEllipse();
//
//	//e->drawToImage(&img, new cv::Scalar(50, 50, 50));
//
//	//char* ellipse = "Edges";
//	//cv::namedWindow(ellipse, CV_WINDOW_AUTOSIZE);
//	//cv::imshow(ellipse, img);
//	//cv::imwrite("..\\testelp.jpg",img);
//	//cv::waitKey(0);*/
//	return 0;
//}