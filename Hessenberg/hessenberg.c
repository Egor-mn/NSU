//
//  hessenberg.c
//  Jacobi
//
//  Created by Egor Manylov on 29.11.15.
//  Copyright © 2015 Egor Manylov. All rights reserved.
//

#include "hessenberg.h"

void inputFileMatrix(matrix m, int *size) {
    FILE * input;
    input = fopen ("input.txt","r");
    
    fscanf(input, "%d", size);
    
    if (input != NULL){
        for (int n = 0; n < *size; n++)
            for (int k = 0; k < *size; k++)
                fscanf(input,"%lf", &m[n][k]);
        
        fclose (input);
    }
    else
        printf("Can't open input file.\n");
}

void printMatrix(matrix m, int size) {
    for (int n = 0; n < size; n++) {
        printf("\n");
        for (int k = 0; k < size; k++)
            printf("%12.5lf", m[n][k]);
        
    }
    printf("\n");
}

void copyMatrix (matrix a, matrix b, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            a[i][j] = b[i][j];
}

void setZeroMatrix (matrix m, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            m[i][j] = 0;
    
}

void setIdentityMatrix (matrix m, int size) {
    setZeroMatrix(m, size);
    
    for (int i = 0; i < size; i++)
        m[i][i] = 1;
}

void subMatrix (matrix a, matrix b, int size, matrix res) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            res[i][j] = a[i][j] - b[i][j];

}

void subVectors (vector a, vector b, int size, vector res) {
    for (int k = 0; k < size; k++)
        res[k] = a[k] - b[k];
}

void multiplyVectors (vector a, vector b, int size, matrix res) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            res[i][j] = a[i] * b[j];
}

void multiplyVectorConst (vector a, double c, int size) {
    for (int k = 0; k < size; k++)
        a[k] = a[k] * c;
}

double findS (matrix m, int k, int size) {
    double s = 0;
    for (int i = k + 1; i < size; i++)
        s += m[i][k] * m[i][k];

    return sqrt(s);
}

double vectorLength (vector a, int size) {
    double sum = 0;
    for (int i = 0; i < size; i++)
        sum += a[i] * a[i];
    
    return sqrt(sum);
}

void multiplyMatrixConst(matrix a, double c, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            a[i][j] *= c;
}

void multiplyMatrix(matrix a, matrix b, int size, matrix res) {
    double sum = 0;
    matrix res1;
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            sum = 0;
            for (int n = 0; n < size; n++)
                sum += a[i][n] * b[n][j];

            res1[i][j] = sum;
        }
    }
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            sum = 0;
            for (int n = 0; n < size; n++)
                sum += res1[i][n] * a[j][n];

            res[i][j] = sum;
        }
    }
}

void getHessenbergMatrix (matrix m, int size, matrix h) {
    matrix I, R, N, res;
    vector x,y,w;
    
    copyMatrix(h, m, size);
    
    for (int k = 0; k < size - 2; k++) {
        setIdentityMatrix(I, size);
        double s = findS(h, k, size);
        double q = - h[k+1][k] / fabs(h[k+1][k]);
        
        for (int j = 0; j < size; j++) {
            x[j] = h[j][k];
            
            if (j <= k) y[j] = h[j][k];
            else y[j] = 0;
        }
        y[k+1] = q * s;
        
        subVectors(x, y, size, w);
        multiplyVectorConst(w, 1 / vectorLength(w, size), size);
        
        multiplyVectors(w, w, size, N);
        multiplyMatrixConst(N, 2, size);
        
        subMatrix(I, N, size, R);
        
        multiplyMatrix(R, h, size, res);
        copyMatrix(h, res, size);
    }
}