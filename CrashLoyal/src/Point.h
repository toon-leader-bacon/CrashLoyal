#pragma once

#include <cmath>
#include <iostream>
#include <stdio.h>

struct Point {
	float x;
	float y;

public: 

	Point() {
		this->x = 0;
		this->y = 0;
	}
	
	Point(int x, int y) {
		this->x = (float)x;
		this->y = (float)y;
	}

	Point(float x, float y) {
		this->x = x;
		this->y = y;
	}

	Point(const Point& other) {
		this->x = other.x;
		this->y = other.y;
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

	Point& operator=(const Point& rhs) { x = rhs.x; y = rhs.y; return *this; }

	Point operator+(const Point& rhs) const { return Point(x + rhs.x, y + rhs.y); }
	Point& operator+=(const Point& rhs) { x += rhs.x; y += rhs.y; return *this; }

	Point operator*(const float f) const { return Point(x * f, y * f); }
	Point& operator*=(const float f) { x *= f; y *= f; return *this; }

	float dist(Point other) {
		return Point::dist(*this, other);
	}

	void normalize() {
		float mag = sqrt((x * x) + (y * y));

		float nearZero = 0.00001f;
		if (mag <= nearZero) { return; }

		this->x = this->x / mag;
		this->y = this->y / mag;
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

