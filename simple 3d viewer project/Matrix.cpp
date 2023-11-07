#include "Matrix.h"

Matrix::Matrix(size_t sR, size_t sC)
	: sizeR(sR), sizeC(sC)
{
	elem = new double* [sizeR];
	for (size_t i = 0; i < sizeR; i++)
	{
		elem[i] = new double[sizeC];
		for (size_t j = 0; j < sizeC; j++)
		{
			elem[i][j] = 0;
		}
	}
	
}

Matrix::~Matrix()
{
	for (size_t i = 0; i < sizeR; i++)
		delete[] elem[i];
	delete[] elem;
}

Coord Matrix::mult(Coord coord)
{
	double x = coord.x;
	double y = coord.y;
	double z = coord.z;

	coord.x = x * elem[0][0] + y * elem[1][0] + z * elem[2][0];
	coord.y = x * elem[0][1] + y * elem[1][1] + z * elem[2][1];
	coord.z = x * elem[0][2] + y * elem[1][2] + z * elem[2][2];

	return coord;
}
