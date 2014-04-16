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
	double operator*(const Point&);
	Point operator+(const Point& );
	Point operator-(const Point&);
	int operator==(const Point&);
	double norm();
private:
	int x, y;
};

#endif