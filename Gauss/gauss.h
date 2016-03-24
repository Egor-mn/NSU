//
//  gauss.h
//  Gauss
//
//  Created by Egor Manylov on 28.02.16.
//  Copyright © 2016 Egor Manylov. All rights reserved.
//

#ifndef gauss_h
#define gauss_h

#include <stdio.h>
#include "math.h"

#define n_max 20
//#define double float

typedef double vector[n_max];
typedef double matrix[n_max][n_max];

uint transp();

double perturbationMethod(int ind, matrix m, vector b, int size);

void inputFileMatrix(matrix m, vector b, int *size);  //especially for old fart

void hilbertMatrix(matrix m, vector y, vector b, int *size);

void keybordMatrix(matrix m, vector b, int *size);

void copyMatrix (matrix a, matrix b, int size);

void multiplyMatrix (matrix a, matrix b, int size, matrix ab);

double matrixDeterminant (matrix A, int size);

void inverseMatrix (matrix A, matrix A1, int size);;

void printMatrix (matrix m, int size);

void printVector (vector v, int size);

void rookPivoting (matrix m, int *i, int *j, int size);  //2.5

void LUdecomposition (matrix A, matrix LU, int size, void (*pivot)(matrix, int *, int *, int));

int GaussianElimination (matrix A, vector x, vector b, int size, void (*pivot)(matrix, int *, int *, int));

void iterativeRefinement (matrix A, vector x, vector b, int size, void (*pivot)(matrix, int *, int *, int));

double getVectorNorm(int ind, vector v, int size);

double getResidualNorm (int id, matrix A, vector x, vector b, int size);

double getRelativeResidualNorm (int id, matrix A, vector x, vector b, int size);

double relativeError (int id, vector x, vector y, int size);

#endif /* gauss_h */
