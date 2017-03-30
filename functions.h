#pragma once
#include<cmath>
#include"basicStructures.h"

typedef long double ld;

const ld eps = 1e-10;

bool isZero(ld z) {
	return fabs(z) < eps;
}

int sign(ld z) {
	if (isZero(z)) return 0;
	return z > 0 ? 1 : -1;
}

bool areEqual(ld x, ld y) {
	return sign(y - x);
}


