//#include "Point.h"
//#include "Defines.h"
//#include <math.h>
//#include <stdio.h>
//#include <iostream>
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//
//using namespace std;
//int main(int argc, char** argv) {
//	Point P(2.5,2);
//	Point L2(1,4.5);
//	Point L1(2,3.5);
//	Point R1(1,1);
//	Point R2(5.5,3.5);
//
//	Point r0 =L1-L2 ;
//	Point r1 = P-L2;
//	Point r2 = R1-L2;
//
//	int a1 = acos((r0 * r1) / (r0.norm()* r1.norm()))*(180 / PI) + .5;
//	int a2 = acos((r0 * r2) / (r0.norm()* r2.norm()))*(180 / PI) + .5;
//	int g1 = acos((r1 *r2) / (r1.norm()* r2.norm()))*(180 / PI) + .5;
//
//	int t1 = acos((r0 * r1) / (r0.norm()* r1.norm()))<<2;
//	//int t2 = acos((r0 * r2) / (r0.norm()* r2.norm()))<<2;
//	int t3 = acos((r1 *r2) / (r1.norm()* r2.norm()))<<2;
//	int aa1 = t1 *(180 / PI) + .5;
//	int aa2 = t2*(180 / PI) + .5;
//	int gg1 = t3*(180 / PI) + .5;
//	getchar();
//
//}