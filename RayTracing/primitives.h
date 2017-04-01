#pragma once
#include"objects.h"
#include"basicStructures.h"
#include"attributes.h"


class Triangle: public Object {
public:
	Triangle(Point& p1, Point& p2, Point& p3, Point& normal, Attributes sight) {
		a_ = p1;
		b_ = p2;
		c_ = p3;
		normal_ = normal;
		attr_ = sight;
	}

	Line reflRay(Line& ray, Point& x) {
		ld proj = ray.direct*normal_ / normal_.len();
		Point newDirect = ray.direct - normal_ / normal_.len() * proj * 2;
		return Line{ x, x + newDirect };
	}

	Point getNormal(Point& x) {
		return normal_;
	}

	ld getReflection() {
		return attr_.reflection;
	}

	bool rayIntersect(Line& ray, Point& result, ld& distance, Colour& color) {
		Point p1 = ray.direct;
		Point p2 = b_ - c_;
		Point p3 = b_ - a_;
		Point p4 = b_ - ray.point;
		ld delta = det(p1, p2, p3);

		if (delta != 0) {
			ld u = det(p1, p4, p3) / delta;
			ld v = det(p1, p2, p4) / delta;
			ld dist = det(p4, p2, p3) / delta;
			distance = dist;
			result = ray.point + ray.direct * dist;
			if (normal_*ray.direct < 0) {
				color = attr_.face;
			}
			else {
				color = attr_.back;
			}
			return (u > 0 || isZero(u)) && (v > 0 || isZero(v)) && (1 - u - v > 0 || isZero(1 - u - v)) && !isZero(distance);
		}
		else {
			return 0;
		}
	}
	~Triangle() {}
private:
	Point a_;
	Point b_;
	Point c_;
	Point normal_;
	Attributes attr_;
};

class Quadrangle : public Object {
public:
	Quadrangle(Point& p1, Point& p2, Point& p3, Point& p4, Point& normal, Attributes sight) {
		a_ = p1;
		b_ = p2;
		c_ = p3;
		d_ = p4;
		normal_ = normal;
		attr_ = sight;
	}

	Line reflRay(Line& ray, Point& x) {
		ld proj = ray.direct*normal_ / normal_.len();
		Point newDirect = ray.direct - normal_ / normal_.len() * proj * 2;
		return Line{ x, x + newDirect };
	}

	ld getReflection() {
		return attr_.reflection;
	}

	Point getNormal(Point& x) {
		return normal_;
	}

	bool rayIntersect(Line& ray, Point& result, ld& distance, Colour& color) {
		Triangle tr1(a_, b_, c_, normal_, attr_);
		Triangle tr2(a_, c_, d_, normal_, attr_);
		if (tr1.rayIntersect(ray, result, distance, color) && !isZero(distance)) {
			return 1;
		}
		else
			if (tr2.rayIntersect(ray, result, distance, color) && !isZero(distance)) {
				return 1;
			}
			else {
				return 0;
			}
	}
	~Quadrangle() {}
private:
	Point a_;
	Point b_;
	Point c_;
	Point d_;
	Point normal_;
	Attributes attr_;
};

class Sphere : public Object {
public:
	Sphere(Point& c, ld r, Attributes sight) {
		cen_ = c;
		rad_ = r;
		attr_ = sight;
	}

	Point getNormal(Point& x) {
		return x - cen_;
	}

	ld getReflection() {
		return attr_.reflection;
	}

	Line reflRay(Line& ray, Point& x) {
		Point normal = getNormal(x);
		ld proj = ray.direct*normal / normal.len();
		Point newDirect = ray.direct - normal / normal.len() * proj * 2;
		return Line{ x, x + newDirect };
	}

	bool rayIntersect(Line& ray, Point& result, ld& distance, Colour& color) {
		Point cent = cen_;
		ld proj = ray.direct*cent / ray.direct.len();
		if (cent.len2() - proj*proj > rad_ * rad_) {
			return 0;
		}
		else {
			ld deltaDist = sqrt(rad_ * rad_ - (cent.len2() - proj*proj));
			distance = (proj - deltaDist) / ray.direct.len();
			if (distance < 0 || isZero(distance)) {
				return 0;
			}
			result = ray.point + ray.direct * distance;
			color = attr_.face;
			return 1;
		}
	}
	~Sphere() {}
private:
	ld rad_;
	Point cen_;
	Attributes attr_;
};