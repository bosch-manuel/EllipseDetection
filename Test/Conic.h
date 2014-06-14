#ifndef CONIC_H
#define CONIC_H

class Conic {
public:
	Conic(double A, double B,double C, double D,double E,double F);
	~Conic();

	double getA();
	double getB();
	double getC();
	double getD();
	double getE();
	double getF();

	bool isEllipse();

	void drawConic();

private:
	double A, B, C, D, E, F;
	bool is_Ellipse;
};

#endif

