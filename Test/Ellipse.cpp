#include "Ellipse.h"
#include "Defines.h"
#include <list>
#include <iostream>



Ellipse::Ellipse(double A, double B, double C, double D, double E, double F) {
	this->A = A;
	this->B = B;
	this->C = C;
	this->D = D;
	this->E = E;
	this->F = F;
	double A_, C_, D_, E_,cost,sint,x0_,y0_;
	theta = atan2(B, A - C)/2;
	cost = cos(theta);
	sint = sin(theta);
	
	A_ = A*cost*cost + B*cost*sint + C*sint*sint;
	C_ = A*sint*sint - B*cost*sint + C*cost*cost;
	D_ = D*cost + E*sint;
	E_ = -D*sint + E*cost;

	b = sqrt((-4 * F*A_*C_ + C_*D_*D_ + A_*E_*E_) / (4 * A_*C_*C_));
	a = sqrt((-4 * F*A_*C_ + C_*D_*D_ + A_*E_*E_) / (4 * A_*A_*C_));

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

double Ellipse::calcAvarageDistances(std::list<Point*> *points) {
	double xi, yi, y1,y2,x1,x2,k,m;
	double dist,d1,d2;
	double sum_dist=0;
#ifdef DEBUG_ELLIP_DIST
	std::cout << "Ellipse: x0,y0,a,b : A B C D E F>> " << " (" << x0 << ")" << " (" << y0 << ")" << "(" << a << ")" << "(" << b << ") : " << "(" << A << ")" << "(" << B 
		<< ")" << "(" << C << ")" << "(" << D << ")" << "(" << E << ")" << "(" << F << ")" << std::endl << std::endl;
#endif
	for (std::list<Point*>::const_iterator i = points->cbegin(); i != points->cend(); i++)	{
		//calc algebraic distance to ellipse
		xi = (*i)->getX();
		yi = (*i)->getY();
		/*parameter of line through M and (xi,yi)*/
		m = (yi - y0) / (xi - x0);
		k = -m*x0 + y0;
		/*Calc intersection between line and ellipse*/
		x1 = (b *b * x0 + a *a * m*y0 + a*b*sqrt(a *a * m *m + b *b - k *k - 2 * k*m*x0 + 2 * k*y0 - m *m * x0 *x0 + 2 * m*x0*y0 - y0 *y0) - a *a * k*m) / (a *a * m*m + b *b);
		x2 = (b *b * x0 + a *a * m*y0 - a*b*sqrt(a *a * m *m + b *b - k *k - 2 * k*m*x0 + 2 * k*y0 - m *m * x0 *x0 + 2 * m*x0*y0 - y0 *y0) - a *a* k*m) / (a *a * m *m + b *b);

		y1 = m*x1 + k;
		y2 = m*x2 + k;

		d1 = sqrt(pow(y1 - yi, 2) + pow(x1 - xi, 2));
		d2 = sqrt(pow(y2 - yi, 2) + pow(x2 - xi, 2));

		dist = std::min(d1, d2);
#ifdef DEBUG_ELLIP_DIST
			std::cout << "xi, yi <> x1 y1 " << "(" << xi << ", " << yi << ")<>" << "(" << x1 << ", " << y1 << ")" << dist << std::endl << std::endl;
#endif
		sum_dist += dist;
	}
	sum_dist /= (points->size());

#ifdef DEBUG_ELLIP_DIST
	std::cout << ">>>>>> " << sum_dist << std::endl << std::endl;
#endif

	return sum_dist;
}

double Ellipse::calcDistanceToPoints(std::list<Point*> *points) {
	int pointsOnEllipse = 2 * points->size();
	double step = (1.5*PI) / pointsOnEllipse;
	double phi = 0;
	double x_phi, y_phi, cos_theta, sin_theta, cos_phi, sin_phi,dist,min_dist,xi,yi,sum_min_dist,x_min,y_min;

	sin_theta = sin(theta);
	cos_theta = cos(theta);
	//calc points on ellipse
#ifdef DEBUG_ELLIP_DIST
	std::cout << "Ellipse: x0,y0,a,b : A B C D E F>> " << " (" << x0 << ")" << " (" << y0 << ")" << "(" << a << ")" << "(" << b << ") : " << "(" << A << ")" << "(" << B
		<< ")" << "(" << C << ")" << "(" << D << ")" << "(" << E << ")" << "(" << F << ")" << std::endl << std::endl;
#endif
	sum_min_dist = 0;
	for (int i = 0; i < pointsOnEllipse; i++) {
		sin_phi = sin(phi);
		cos_phi = cos(phi);
		x_phi = x0 + a*cos_phi*cos_theta - b*sin_phi*sin_theta;
		y_phi = y0 + b*cos_phi*sin_theta + B*sin_phi*cos_theta;

#ifdef DEBUG_ELLIP_DIST
		std::cout << phi*(180/PI)<<"(" << x_phi << "," << y_phi << ")" << std::endl << std::endl;
#endif

		//calc min distance 
		min_dist = 1000000000000000000;
		for (std::list<Point*>::const_iterator j = points->cbegin(); j != points->cend(); j++)	{
			xi = (*j)->getX();
			yi = (*j)->getY();
			dist = sqrt(pow(y_phi - yi, 2) + pow(x_phi - xi, 2));
			if (dist < min_dist) {
				min_dist = dist;
				x_min = xi;
				y_min = yi;
			}
		}
#ifdef DEBUG_ELLIP_DIST
		std::cout << "Min Dist: " << "(" << x_min << "," << y_min << ")" <<">>>" << min_dist << std::endl << std::endl;
#endif
		sum_min_dist += min_dist;
		phi += step;
	}
	

#ifdef DEBUG_ELLIP_DIST
	std::cout << "Sum Min Dist >>>" << sum_min_dist << std::endl << std::endl;
#endif
	sum_min_dist /= (pointsOnEllipse);
#ifdef DEBUG_ELLIP_DIST
	std::cout << "Sum Min Dist geteilt >>>" << sum_min_dist << std::endl << std::endl;
#endif
	return sum_min_dist;
}

void Ellipse::drawToImage(cv::Mat *img,cv::Scalar *color) {
	cv::Point center(x0, y0);
	cv::Size size(a, b);
	cv::ellipse(*img, center, size, theta*(180/PI), 0, 360, *color,1,8,0);
}
