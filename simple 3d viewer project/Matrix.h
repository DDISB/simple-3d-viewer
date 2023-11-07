#pragma once
#include "coord.h"

class Matrix
{
public:
	Matrix(size_t sR, size_t sC);
	~Matrix();
	Coord mult(Coord coord);
	double& operator () (unsigned int x, unsigned int y) { return elem[x][y]; }

private:
	size_t sizeR;
	size_t sizeC;
	double** elem;
};