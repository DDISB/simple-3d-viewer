export module Matrix;

export class Matrix
{
public:
	Matrix(size_t sR, size_t sC);
	~Matrix();
	
private:
	size_t sizeR;
	size_t sizeC;
	double** elem;
};

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

