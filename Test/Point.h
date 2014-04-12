#ifndef POINT_H
#define POINT_H

class Point {
public:
	/*!! y,x !!*/
	Point(int, int);
	~Point();

	int getX();
	int getY();
	/*scalar product*/
	double operator*(Point);
	Point operator+(Point);
	Point operator-(Point);
	double norm();
private:
	int x, y;
};

#endif