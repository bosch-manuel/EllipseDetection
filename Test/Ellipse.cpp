#include "Ellipse.h"
#include "Defines.h"
#include <list>
#include <iostream>
#include "Eigenvalues"



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

/*
Based on optimized MAtlab code from "Direct Least Squares Fitting for Ellipses"
http://research.microsoft.com/en-us/um/people/awf/ellipse/fitellipse.html
*/
Ellipse* calcEllipse(std::vector<Point*> *points) {
	using namespace std;
	if (points->size() < 6) {
		return NULL; //Error too few points
	}
	double mx, my, sx, sy, xmax, xmin, ymax, ymin, x, y;
	int ind = 0;
	int i_neg_eigenval;
	mx = 0;
	my = 0;
	xmax = 0;
	ymax = 0;
	xmin = FLT_MAX;
	ymin = FLT_MAX;
	for (std::vector<Point*>::const_iterator i = points->cbegin(); i != points->cend(); i++)	{
		//(row,col)
		x = (*i)->getX();
		y = (*i)->getY();
		mx += x;
		my += y;
		if (x > xmax) {
			xmax = x;
		}
		if (x < xmin) {
			xmin = x;
		}
		if (y > ymax) {
			ymax = y;
		}
		if (y < ymin) {
			ymin = y;
		}
		ind++;
	}
	//normalize data
	//mx = mean(X);
	//my = mean(Y);
	//sx = (max(X) - min(X)) / 2;
	//sy = (max(Y) - min(Y)) / 2;
	mx /= points->size();
	my /= points->size();
	sx = (xmax - xmin) / 2;
	sy = (ymax - ymin) / 2;

#ifdef DEBUG_CALC
	cout << "mx= " << mx << endl << "my= " << my << "sx= " << sx << endl << "sy= " << sy << endl;
	cout << "maxx= " << xmax << endl << "maxy= " << ymax << endl;
	cout << "minx= " << xmin << endl << "miny= " << ymin << endl;
#endif

	Eigen::MatrixXd D(points->size(), 6);
	Eigen::MatrixXd C(6, 6);
	Eigen::MatrixXd tmpA(3, 3);
	Eigen::MatrixXd tmpB(3, 3);
	Eigen::MatrixXd tmpD(3, 3);
	Eigen::MatrixXd tmpC(3, 3);
	Eigen::MatrixXd S, tmp, tmpE, tmp1, tmp2, evec_y;

	Eigen::VectorXd A(6);
	Eigen::VectorXd a(6);

	//% Build design matrix
	//D = [x.*x  x.*y  y.*y  x  y  ones(size(x))];
	int index = 0;
	for (std::vector<Point*>::const_iterator i = points->cbegin(); i != points->cend(); i++)	{
		//D(row,col)
		//normalize step 2
		//x = (X - mx) / sx;
		//y = (Y - my) / sy;
		x = ((*i)->getX() - mx) / sx;
		y = ((*i)->getY() - my) / sy;
		D(index, 0) = x*x;
		D(index, 1) = x*y;
		D(index, 2) = y*y;
		D(index, 3) = x;
		D(index, 4) = y;
		D(index, 5) = 1;

		index++;
	}

	/*% Build scatter matrix
	S = D'*D;*/
	S = D.transpose()*D;

#ifdef DEBUG_CALC
	cout << "S=" << endl << S << endl << endl;
#endif
	/*% Build 6x6 constraint matrix
	C(6, 6) = 0; C(1, 3) = -2; C(2, 2) = 1; C(3, 1) = -2;*/
	C = C.Zero(6, 6);
	C(0, 2) = -2;
	C(1, 1) = 1;
	C(2, 0) = -2;

	//% New way, numerically stabler in C[gevec, geval] = eig(S, C);
#ifdef DEBUG_CALC
	cout << "C=" << endl << C << endl << endl;
#endif
	/*% Break into blocks*/
	//tmpA = S(1:3, 1 : 3);
	tmpA(0, 0) = S(0, 0);		tmpA(0, 1) = S(0, 1);		tmpA(0, 2) = S(0, 2);
	tmpA(1, 0) = S(1, 0);		tmpA(1, 1) = S(1, 1);		tmpA(1, 2) = S(1, 2);
	tmpA(2, 0) = S(2, 0);		tmpA(2, 1) = S(2, 1);		tmpA(2, 2) = S(2, 2);

	//tmpB = S(1:3, 4 : 6);
	tmpB(0, 0) = S(0, 3);		tmpB(0, 1) = S(0, 4);		tmpB(0, 2) = S(0, 5);
	tmpB(1, 0) = S(1, 3);		tmpB(1, 1) = S(1, 4);		tmpB(1, 2) = S(1, 5);
	tmpB(2, 0) = S(2, 3);		tmpB(2, 1) = S(2, 4);		tmpB(2, 2) = S(2, 5);

	//tmpC = S(4:6, 4 : 6);
	tmpC(0, 0) = S(3, 3);		tmpC(0, 1) = S(3, 4);		tmpC(0, 2) = S(3, 5);
	tmpC(1, 0) = S(4, 3);		tmpC(1, 1) = S(4, 4);		tmpC(1, 2) = S(4, 5);
	tmpC(2, 0) = S(5, 3);		tmpC(2, 1) = S(5, 4);		tmpC(2, 2) = S(5, 5);

	//tmpD = C(1:3, 1 : 3);
	tmpD(0, 0) = C(0, 0);		tmpD(0, 1) = C(0, 1);		tmpD(0, 2) = C(0, 2);
	tmpD(1, 0) = C(1, 0);		tmpD(1, 1) = C(1, 1);		tmpD(1, 2) = C(1, 2);
	tmpD(2, 0) = C(2, 0);		tmpD(2, 1) = C(2, 1);		tmpD(2, 2) = C(2, 2);

#ifdef DEBUG_CALC
	cout << "tmpA=" << endl << tmpA << endl << endl;
	cout << "tmpB=" << endl << tmpB << endl << endl;
	cout << "tmpC=" << endl << tmpC << endl << endl;
	cout << "tmpD=" << endl << tmpD << endl << endl;
#endif
	//tmpE = inv(tmpC)*tmpB';
	tmpE = tmpC.inverse()*tmpB.transpose();

#ifdef DEBUG_CALC
	cout << "tmpE=" << endl << tmpE << endl << endl;
#endif

	//[evec_x, eval_x] = eig(inv(tmpD) * (tmpA - tmpB*tmpE)); 
	tmp1 = tmpB*tmpE;
	tmp2 = tmpA - tmp1;
	tmp = tmpD.inverse()*tmp2;

#ifdef DEBUG_CALC
	cout << "tmp1=" << endl << tmp1 << endl << endl;
	cout << "tmp2=" << endl << tmp2 << endl << endl;
	cout << "tmp=" << endl << tmp << endl << endl;
#endif

	Eigen::EigenSolver<Eigen::MatrixXd> es(tmp);
	Eigen::MatrixXd eigenval = es.eigenvalues().real();
	Eigen::MatrixXd eigenvec = es.eigenvectors().real();

#ifdef DEBUG_CALC
	cout << "val=" << endl << eigenval << endl << endl;
	cout << "val rows=" << eigenval.rows() << endl << "val cols=" << eigenval.cols() << endl;
	cout << "vec=" << endl << eigenvec << endl << endl;
#endif

	//% Find the negative(as det(tmpD) < 0) eigenvalue
	//I = find(real(diag(eval_x)) < 1e-8 & ~isinf(diag(eval_x)));
	for (int i = 0; i < eigenval.rows(); i++)	{
		if (eigenval(i, 0) < 0) {
			i_neg_eigenval = i;
			break;
		}
	}

#ifdef DEBUG_CALC
	cout << "Negative eigenvalue: " << eigenval(i_neg_eigenval, 0) << endl << endl;
#endif

	/*% Extract eigenvector corresponding to negative eigenvalue
	A = real(evec_x(:, I));*/
	Eigen::VectorXd at = eigenvec.col(i_neg_eigenval).real();

#ifdef DEBUG_CALC
	cout << "coresponding eigenvector: " << endl << at << endl << endl;
#endif

	/*% Recover the bottom half...
	evec_y = -tmpE * A;
	A = [A; evec_y];*/
	evec_y = -tmpE*at;
#ifdef DEBUG_CALC
	cout << " Evec_y: " << endl << evec_y << "  " << evec_y.rows() << "  " << evec_y.cols() << endl << endl;
#endif
	A << at(0), at(1), at(2), evec_y(0), evec_y(1), evec_y(2);
#ifdef DEBUG_CALC
	cout << " A: " << endl << A << endl << endl;
#endif

	/*% unnormalize
	par = [
	A(1)*sy*sy, ...
	A(2)*sx*sy, ...
	A(3)*sx*sx, ...
	- 2 * A(1)*sy*sy*mx - A(2)*sx*sy*my + A(4)*sx*sy*sy, ...
	- A(2)*sx*sy*mx - 2 * A(3)*sx*sx*my + A(5)*sx*sx*sy, ...
	A(1)*sy*sy*mx*mx + A(2)*sx*sy*mx*my + A(3)*sx*sx*my*my   ...
	- A(4)*sx*sy*sy*mx - A(5)*sx*sx*sy*my   ...
	+ A(6)*sx*sx*sy*sy   ...
	]';*/
	a(0) = A(0)* sy*sy;
	a(1) = A(1)*sx*sy;
	a(2) = A(2)*sx*sx;
	a(3) = -2 * A(0)*sy*sy*mx - A(1)*sx*sy*my + A(3)*sx*sy*sy;
	a(4) = -A(1)*sx*sy*mx - 2 * A(2)*sx*sx*my + A(4)*sx*sx*sy;
	a(5) = A(0)*sy*sy*mx*mx + A(1)*sx*sy*mx*my + A(2)*sx*sx*my*my - A(3)*sx*sy*sy*mx - A(4)*sx*sx*sy*my + A(5)*sx*sx*sy*sy;

#ifdef DEBUG_CALC
	cout << " a: " << endl << a << endl << endl;
#endif

	//calculate if the ellipse is a true ellipse for the given pixels
	Ellipse *e = new Ellipse(a(0), a(1), a(2), a(3), a(4), a(5));
	
	return e;

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

double Ellipse::calcDistance(Point *p) {
	double xi, yi, y1, y2, x1, x2, k, m;
	double dist, d1, d2;
	double sum_dist = 0;
#ifdef DEBUG_ELLIP_DIST
	std::cout << "Ellipse: x0,y0,a,b : A B C D E F>> " << " (" << x0 << ")" << " (" << y0 << ")" << "(" << a << ")" << "(" << b << ") : " << "(" << A << ")" << "(" << B
		<< ")" << "(" << C << ")" << "(" << D << ")" << "(" << E << ")" << "(" << F << ")" << std::endl << std::endl;
#endif
	
		//calc algebraic distance to ellipse
		xi = p->getX();
		yi = p->getY();
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

	return dist;
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

std::vector<int>* Ellipse::getIndexesOfMatchingPoints(std::vector<Point*> *points){
	std::vector<int> *indexes = new std::vector<int>;
	int j = 0;
	double dist = -1;
	for (int i = 0; i <points->size(); i++) {
		//calc distance for every point; a point matches the ellipse if its distance is smaller TH_DIST
		if (calcDistance(points->at(i)) <= TH_DIST) {
			indexes->push_back(i);
			j++;
		}
	}

	return indexes;
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
