#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Point.h"
#include <list>
#include <ostream>

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

	double calcDistance(Point *p);

	double calcAvarageDistances(std::list<Point*> *points);
	double calcDistanceToPoints(std::list<Point*> *points);

	friend std::ostream& operator<<(std::ostream& os, Ellipse& e);
	/*Get the indexes of those points which matches this ellipse
		param:
			points		considered points
			
		return:
			vector with indexes of matching points*/
	std::vector<int>* getIndexesOfMatchingPoints(std::vector<Point*> *points);
	
private:
	double A, B, C, D, E, F;
	double x0;
	double y0;
	double a;
	double b;
	double theta;
};
//std::ostream& operator<<(std::ostream& os, Ellipse& e) {
//	E
//}

Ellipse* calcEllipse(std::vector<Point*> *points);
#endif

