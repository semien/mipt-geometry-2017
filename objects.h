#pragma once
#include"basicStructures.h"
#include"attributes.h"
#include"functions.h"


class Object {
public:
	virtual bool rayIntersect(Line& ray, Point& result, ld& distance, Colour& color) = 0;
	virtual ~Object() {}
};
