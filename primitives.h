#pragma once
#include"objects.h"
#include"basicStructures.h"
#include"attributes.h"


class Triangle: public Object {
public:
	Triangle(Point& p1, Point& p2, Point& p3, Point& normal, Attributes sight) {
		a = p1;
		b = p2;
		c = p3;
		this->normal = normal;
		attr = sight;
	}
	Point a;
	Point b;
	Point c;
	Point normal;
	Attributes attr;

	bool rayIntersect(Line& ray, Point& result, ld& distance, Colour& color) {
		Point p1 = ray.direct;
		Point p2 = b - c;
		Point p3 = b - a;
		Point p4 = b - ray.point;
		ld delta = det(p1, p2, p3);

		if (delta != 0) {
			ld u = det(p1, p4, p3) / delta;
			ld v = det(p1, p2, p4) / delta;
			ld dist = det(p4, p2, p3) / delta;
			distance = dist;
			result = ray.point + ray.direct * dist;
			if (normal*ray.direct < 0) {
				color = attr.face;
			}
			else {
				color = attr.back;
			}
			return (u >= 0) && (v >= 0) && (1 - u - v >= 0);
		}
		else {
			return 0;
		}
	}
	~Triangle() {}
};

class Quadrangle : public Object {
public:
	Quadrangle(Point& p1, Point& p2, Point& p3, Point& p4, Point& normal, Attributes sight) {
		a = p1;
		b = p2;
		c = p3;
		d = p4;
		this->normal = normal;
		attr = sight;
	}
	Point a;
	Point b;
	Point c;
	Point d;
	Point normal;
	Attributes attr;
	bool rayIntersect(Line& ray, Point& result, ld& distance, Colour& color) {
		Triangle tr1(a, b, c, normal, attr);
		Triangle tr2(a, c, d, normal, attr);
		if (tr1.rayIntersect(ray, result, distance, color)) {
			return 1;
		}
		else
			if (tr2.rayIntersect(ray, result, distance, color)) {
				return 1;
			}
			else {
				return 0;
			}
	}
	~Quadrangle() {}
};

class Sphere : public Object {
public:
	Sphere(Point& c, ld r, Attributes sight) {
		cen = c;
		rad = r;
		attr = sight;
	}
	ld rad;
	Point cen;
	Attributes attr;
	bool rayIntersect(Line& ray, Point& result, ld& distance, Colour& color) {
		Point cent = cen;
		ld proj = ray.direct*cent / ray.direct.len();
		if (cent.len2() - proj*proj > rad * rad) {
			return 0;
		}
		else {
			ld deltaDist = sqrt(rad * rad - (cent.len2() - proj*proj));
			distance = (proj - deltaDist) / ray.direct.len();
			result = ray.point + ray.direct * distance;
			color = attr.face;
			return 1;
		}
	}
	~Sphere() {}
};