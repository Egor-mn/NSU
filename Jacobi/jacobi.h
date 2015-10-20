//
//  jacobi.h
//  Jacobi
//
//  Created by Егор Манылов on 14.10.15.
//  Copyright © 2015 Egor_Mn. All rights reserved.
//

#ifndef jacobi_h
#define jacobi_h
#endif /* jacobi_h */

#include "stdio.h"
#include "math.h"

#define n_max 10
#define max_iteration 20
#define eps 1e-6

typedef double vector[n_max];
typedef double matrix[n_max][n_max];

void inputFileMatrix(matrix m, int *size);

void hilbertMatrix(matrix m, int *size);

void keybordMatrix(matrix m, int *size);

void setZeroMatrix (matrix m, int size);

void setIdentityMatrix (matrix m, int size);

void copyMatrix (matrix a, matrix b, int size);

void printMatrix(matrix m, int size);

void printDiagonal(matrix m, int size);

void multiplyRotationMatrix(matrix a, matrix eigen_vectors, int i, int j, int size);

void multiplyVectorConst(vector a, double c, int size);

void multiplyMatrixVector(matrix m, vector v, int size, vector res);

void subVectors(vector a, vector b, int size, vector res);

double getDotProduct(vector a, vector b, int size);

double getMaxOffDiagonal(matrix m, int size);

double getAverageOffDiagonal(matrix m, int size);

double getTraceMatrix(matrix m, int size);

double getVectorNorm(int ind, vector v, int size);

double getResidualNorm(int id, matrix a, vector x, vector y, double c, int size);

double getRayleighQuotient(int id, matrix a, matrix eigen_vectors, int size);

int roundRobin(matrix m, matrix eigen_vectors, int size);

void analysisOfResults(matrix m, matrix orig, matrix eigen_vectors, int size);

