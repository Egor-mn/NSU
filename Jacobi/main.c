//
//  main.c
//  Jacobi
//
//  Created by Егор Манылов on 06.10.15.
//  Copyright © 2015 Egor_Mn. All rights reserved.
//

#include "stdio.h"
#include "jacobi.h"

int main() {
    
    matrix m, orig;
    matrix eigen_vectors;
    
    int size, c;
    
    printf("Enter matrix from: (1) file, (2) keyboard, (3) gilbert? ");
    scanf("%d", &c);
    
    switch (c) {
        case 1: inputFileMatrix(m, &size); break;
        case 2: keybordMatrix(m, &size); break;
        case 3: hilbertMatrix(m, &size); break;
        default: return 0;
    }
    
    printf("Output: (1) file, (2) display? ");
    scanf("%d", &c);
    
    if (c == 1) {
        FILE * output;
        output = fopen("output.txt","w");
        stdout = output;
    }
    
    printf("\nInput matrix");
    printMatrix(m, size);
    
    copyMatrix(orig, m, size);
    setIdentityMatrix(eigen_vectors, size);
    
    int iterations = roundRobin(m, eigen_vectors, size);
    
    printf("\n------------------------------------");
    printf("\nAmount iterations: %d\n", iterations);
    printf("\nEigenvalues:\n");
    printDiagonal(m, size);
    printf("\nEigenvectors:");
    printMatrix(eigen_vectors, size);
    
    if (c != 1) analysisOfResults(m, orig, eigen_vectors, size);
        
    return 0;
}