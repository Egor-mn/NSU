//
//  main.c
//  Jacobi
//
//  Created by Егор Манылов on 06.10.15.
//  Copyright © 2015 Egor_Mn. All rights reserved.
//

#include "hessenberg.h"

int main() {
    
    matrix m, h;
    int size;
    
    inputFileMatrix(m, &size);
    
    getHessenbergMatrix(m, size, h);
    printMatrix(h, size);
    
    return 0;
}