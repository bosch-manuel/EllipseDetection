#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Point.h"
#include <list>

class Ellipse
{
public:
	/*Calculates the standard ellipse parameters a,b,x0,y0,theta from the coefficients A,B,C,D,E,F of a conic
		param
			A,B,C,D,E,F		coeffs of the conic*/
	Ellipse(double A, double B, double C, double D, double E, double F);
	~Ellipse();
	double getX0();
	double getY0();
	double geta();
	double getb();
	double getTheta();

	void drawToImage(cv::Mat *img,cv::Scalar *color);

	double calcSumOfDistances(std::list<Point*> *points);
private:
	double x0;
	double y0;
	double a;
	double b;
	double theta;
};
#endif

