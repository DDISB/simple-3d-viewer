#pragma once

class Matrix
{
public:
	Matrix(size_t sR, size_t sC);
	~Matrix();

private:
	size_t sizeR;
	size_t sizeC;
	double** elem;
};