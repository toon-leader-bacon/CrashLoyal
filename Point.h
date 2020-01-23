#ifndef __POINT_H__
#define __POINT_H__

#include <cmath>
#include <iostream>

struct Point {
	float x;
	float y;

public: 

	Point() {
		this->x = 0;
		this->y = 0;
	}

	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}

	static Point midpoint(Point a, Point b) {
		Point result = *(new Point());
		result.x = (a.x + b.x) / 2;
		result.y = (a.y + b.y) / 2;
		return result;
	}

	static float dist(Point a, Point b) {
		float xDist = b.x - a.x;
		float yDist = b.y - a.y;
		float result = sqrt(xDist * xDist + yDist * yDist);

		return result;
	}

	float dist(Point other) {
		return Point::dist(*this, other);
	}

	void normalize() {
		float mag = sqrt(x * x + y * y);
		this->x = this->x / mag;
		this->y = this->y / mag;
	}

	void multiply(float a) {
		this->x *= a;
		this->y *= a;
	}

	static bool insideOf(Point a, float aRadius, Point b) {
		// Returns true if the distance between a and b is less than aRadius
		// In other words, true => point b is inside of point a+raidus.
		return dist(a, b) < aRadius;
	}

	bool insideOf(Point other, float radius) {
		return insideOf(*this, radius, other);
	}
};

#endif