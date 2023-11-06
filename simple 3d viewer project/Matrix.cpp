#include "Matrix.h"

Matrix::Matrix(size_t sR, size_t sC)
	: sizeR(sR), sizeC(sC)
{
	elem = new double* [sizeR];
	for (size_t i = 0; i < sizeR; i++)
		elem[i] = new double[sizeC];
}

Matrix::~Matrix()
{
	for (size_t i = 0; i < sizeR; i++)
		delete[] elem[i];
	delete[] elem;
}