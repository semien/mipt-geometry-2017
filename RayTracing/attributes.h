#pragma once

typedef long double ld;

struct Colour {
	double red;
	double green;
	double blue;

	Colour operator*(double c) {
		return Colour{ red*c,green*c,blue*c };
	}

	Colour operator+(Colour& other) {
		return Colour{ red + other.red,green + other.green,blue + other.blue };
	}
};

class Attributes {
public:
	Attributes() {}
	Attributes(Colour faceCol, Colour backCol, double refl) :
		face(faceCol)
		, back(backCol)
		, reflection(refl/100)
	{}
	Colour face;
	Colour back;
	double reflection;
	~Attributes() {}
};