#pragma once

typedef long double ld;

struct Colour {
	ld red;
	ld green;
	ld blue;
};

class Attributes {
public:
	Attributes() {}
	Attributes(Colour faceCol, Colour backCol) :
		face(faceCol)
		, back(backCol)
	{}
	Colour face;
	Colour back;
	~Attributes() {}
};