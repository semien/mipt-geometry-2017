#pragma once
#include"basicStructures.h"
#include"attributes.h"
#include"functions.h"


class Object {
public:
	virtual bool rayIntersect(Line& ray, Point& result, ld& distance, Colour& color) = 0;
	virtual Line reflRay(Line& ray, Point& x) = 0;
	virtual Point getNormal(Point& x) = 0;
	virtual ld getReflection() = 0;
	virtual ~Object() {}
};

struct Light {
	Point a;
	ld intensity;
};