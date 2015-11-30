//
//  hessenberg.h
//  Jacobi
//
//  Created by Egor Manylov on 29.11.15.
//  Copyright © 2015 Egor Manylov. All rights reserved.
//

#ifndef hessenberg_h
#define hessenberg_h

#include <stdio.h>
#include "math.h"

#define n_max 20
#define max_iteration 1000000
#define eps 1e-10

typedef double vector[n_max];
typedef double matrix[n_max][n_max];

void inputFileMatrix(matrix m, int *size);
void printMatrix(matrix m, int size);
void getHessenbergMatrix(matrix m, int size, matrix h);

#endif /* hessenberg_h */
