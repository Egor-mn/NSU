//
//  main.c
//  Gauss
//
//  Created by Egor Manylov on 26.02.16.
//  Copyright © 2016 Egor Manylov. All rights reserved.
//

#include "gauss.h"

int main() {
    matrix A, A1, LU;
    vector x, y, b;
    int size, option;
    
    printf("Enter matrix from: (1) file, (2) keyboard, (3) gilbert? ");
    scanf("%d", &option);
    
    switch (option) {
        case 1: inputFileMatrix(A, b, &size); break;
        case 2: keybordMatrix(A, b, &size); break;
        case 3: hilbertMatrix(A, y, b, &size); break;
        default: return 0;
    }
    
    printf("\nInput matrix:");
    printMatrix(A, size);
    
    printf("\nInput vector b = ");
    printVector(b, size);
    
    LUdecomposition(A, LU, size, rookPivoting);
    printf("\nLU decomposition:");
    printMatrix(LU, size);
    
    if (!GaussianElimination(A, x, b, size, rookPivoting)) {
        printf("singular matrix\n");
        return 1;
    }
    for (int i = 0; i < 3; i++)
        iterativeRefinement(A, x, b, size, rookPivoting);
    
    printf("\nSolution x = ");
    printVector(x, size);
    
    printf("\nAbsolute residual: %e\nRelative residual: %e\n", getResidualNorm(1, A, x, b, size), getRelativeResidualNorm(1, A, x, b, size));
    
    printf("\nMatrix determinant: %lf \n", matrixDeterminant(A, size));
    
    inverseMatrix(A, A1, size);
    
    printf("\nInverse matrix:");
    printMatrix(A1, size);
}


