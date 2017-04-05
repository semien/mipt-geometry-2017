#pragma once
#include<math.h>
#include"primitives.h"
#include"functions.h"

class Point {
public:
	ld x;
	ld y;
	ld z;
	
	Point() : x(0), y(0), z(0) {}
	Point(ld a, ld b, ld c) : x(a), y(b), z(c) {}
	Point(Point& other) : x(other.x), y(other.y), z(other.z) {}

	ld len2() {
		return x*x + y*y + z*z;
	}

	ld len() {
		return sqrtl(len2());
	}

	Point operator+(const Point p2) {
		return Point(x + p2.x, y + p2.y, z + p2.z);
	}

	Point operator-(const Point& p2) {
		return Point(x - p2.x, y - p2.y, z - p2.z);
	}

	Point operator*(ld c) {
		return Point(x*c, y*c, z*c);
	}

	Point operator/(ld c) {
		return Point(x/c, y/c, z/c);
	}

	ld operator*(Point& p2) {
		return x*p2.x + y*p2.y + z*p2.z;
	}

	Point operator^(Point& p2) {
		return Point(y*p2.z - z*p2.y, z*p2.x - x*p2.z, x*p2.y - y*p2.x);
	}
	~Point() {}
};


ld det(Point& p1, Point& p2, Point& p3) {
	return p1.x * p2.y * p3.z + p1.y * p2.z * p3.x + p1.z * p2.x * p3.y -
		p1.x * p2.z * p3.y - p1.y * p2.x * p3.z - p1.z * p2.y * p3.x;
}

struct Line {
	Point point;
	Point direct;
};