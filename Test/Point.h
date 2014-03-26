#ifndef POINT_H
#define POINT_H

class Point {
public:
	Point(int, int);
	~Point();

	int getX();
	int getY();

private:
	int x, y;
};

#endif