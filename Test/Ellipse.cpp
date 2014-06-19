#include "Ellipse.h"
#include "Defines.h"
#include <list>
#include <iostream>



Ellipse::Ellipse(double A, double B, double C, double D, double E, double F) {
	double A_, C_, D_, E_,cost,sint,x0_,y0_;
	theta = atan2(B, A - C)/2;
	cost = cos(theta);
	sint = sin(theta);
	
	A_ = A*cost*cost + B*cost*sint + C*sint*sint;
	C_ = A*sint*sint - B*cost*sint + C*cost*cost;
	D_ = D*cost + E*sint;
	E_ = -D*sint + E*cost;

	a = sqrt((-4 * F*A_*C_ + C_*D_*D_ + A_*E_*E_) / (4 * A_*C_*C_));
	b = sqrt((-4 * F*A_*C_ + C_*D_*D_ + A_*E_*E_) / (4 * A_*A_*C_));

	x0_ = -D_ / (2 * A_);
	y0_ = -E_ / (2 * C_);
	x0 = x0_*cost - y0_*sint;
	y0 = x0_*sint + y0_*cost;
}


Ellipse::~Ellipse()
{
}

double Ellipse::getX0(){
	return x0;
}

double Ellipse::getY0() {
	return y0;
}

double Ellipse::geta() {
	return a;
}

double Ellipse::getb() {
	return b;
}

double Ellipse::getTheta() {
	return theta;
}

double Ellipse::calcSumOfDistances(std::list<Point*> *points) {
	double xi, yi, y1,y2,dyiy1,dyiy2;
	double dist;
	double sum_dist=0;
#ifdef DEBUG_ELLIP_DIST
	std::cout << "Ellipse: x0,y0,a,b>> " << " (" << x0 << ")" << " (" << y0 << ")" << "(" << a << ")" << "(" << b << ")" << std::endl << std::endl;
#endif
	for (std::list<Point*>::const_iterator i = points->cbegin(); i != points->cend(); i++)	{
		//calc algebraic distance to ellipse
		xi = (*i)->getX();
		yi = (*i)->getY();
		y1 = y0+(b/a)*sqrt(a*a-(xi-x0)*(xi-x0));
		y2 = y0 - (b / a)*sqrt(a*a - (xi - x0)*(xi - x0));
		dyiy1 = abs(yi - y1);
		dyiy2 = abs(yi - y2);
		dist = std::min(dyiy1, dyiy2);
		if (dist < -DBL_MAX) {
			dist = 0;
		}
#ifdef DEBUG_ELLIP_DIST
		std::cout << "xi, yi (" << xi << ")"<<"("<<yi<<") - ("<<y1<<", "<<y2<<") : " << dist << std::endl << std::endl;
#endif
		sum_dist += dist;
	}

#ifdef DEBUG_ELLIP_DIST
	std::cout << ">>>>>> " << sum_dist/points->size() << std::endl << std::endl;
#endif
	return sum_dist;
}

void Ellipse::drawToImage(cv::Mat *img,cv::Scalar *color) {
	cv::Point center(x0, y0);
	cv::Size size(a, b);
	cv::ellipse(*img, center, size, theta*(180/PI), 0, 360, *color,1,8,0);
}
