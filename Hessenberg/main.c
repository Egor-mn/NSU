//
//  main.c
//  Jacobi
//
//  Created by Егор Манылов on 06.10.15.
//  Copyright © 2015 Egor_Mn. All rights reserved.
//

#include "hessenberg.h"

int main() {
    
    matrix m, m1, h, store;
    int size;
    
    inputFileMatrix(m, &size);
    
    getHessenbergMatrix(m, size, h, store);
    printMatrix(h, size);
    
    getHessenbergMatrixBack(h, size, m1, store);
    printMatrix(m1, size);
    
    printf("\ndelta = %.20lf\n", getRelativeError(m, m1, size));
    
    return 0;
}