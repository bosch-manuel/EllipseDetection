#ifndef POINT_H
#define POINT_H

class Point {
public:
	/*!! y,x !!*/
	Point(int, int);
	~Point();

	int getX();
	int getY();

private:
	int x, y;
};

#endif