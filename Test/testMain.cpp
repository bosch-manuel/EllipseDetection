#include "Point.h"
#include "Defines.h"
#include "EdgeSegment.h"
#include "EllipticalArc.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
int main(int argc, char** argv) {
	Point P(1,1);
	Point L2(74,430);
	Point L1(75,421);
	Point R1(75,416);
	Point R2(69,406);

	Point r0 = (R1 - R2);
	Point r1 = (L2 - L1);
	double a1 = acos((r0 * r1) / (r0.norm()* r1.norm()))*(180 / PI);
	
	EdgeSegment e1;
	EdgeSegment e2;
	EdgeSegment e3;

	EllipticalArc arc;
	arc.addSegment(&e2);
	arc.addSegment(&e1);
	bool b = arc.containsSegment(&e2);
	bool c = arc.containsSegment(&e3);
	getchar();

}