//
//  gauss.c
//  Gauss
//
//  Created by Egor Manylov on 28.02.16.
//  Copyright © 2016 Egor Manylov. All rights reserved.
//

#include "gauss.h"

int row[n_max], col[n_max];
uint transpositions;

/*void inputFileMatrix(matrix m, vector b, int *size) {
    FILE * input;
    input = fopen ("input.txt","r");
    
    fscanf(input, "%d", size);
    
    if (input != NULL){
        for (int n = 0; n < *size; n++)
            for (int k = 0; k < *size; k++)
                fscanf(input,"%lf", &m[n][k]);
        
        for (int n = 0; n < *size; n++)
            fscanf(input,"%lf", &b[n]);
        
        fclose (input);
    }
    else
        printf("Can't open input file.\n");
}*/

void inputFileMatrix(matrix m, vector b, int *size) {
    FILE * input;
    input = fopen ("input.txt","r");
    
    fscanf(input, "%d", size);
    
    int column = 0;
    
    if (input != NULL){
        while (*size - column > 0) {
            for (int row_ = 0; row_ < *size; row_++) {
                for (int j = 0; j < *size - column && j < 3; j++) {
                    fscanf(input, "%lf", &m[row_][j + 3*(column/3)]);
                }
            }
            column += 3;
        }
        
        for (int n = 0; n < *size; n++)
            fscanf(input,"%lf", &b[n]);
        
        fclose (input);
    }
    else
        printf("Can't open input file.\n");
}

void multiplyMatrixVector (matrix m, vector v, int size, vector res);

void hilbertMatrix(matrix m, vector y, vector b, int *size) {
    int s = 0; //is given by the teacher
    double C;
    
    printf("Enter size of matrix: ");
    scanf("%d", size);
    
    for (int i = 0; i < *size; i++)
        for (int j = 0; j < *size; j++)
            m[i][j] = 1.0 / (i + j + 1 - s);
    
    printf("Enter constant C: ");
    scanf("%lf", &C);
    for (int i = 0; i < *size; i++)
        y[i] = (i + 1) * C;
    
    multiplyMatrixVector(m, y, *size, b);
}

void keybordMatrix(matrix m, vector b, int *size) {
    printf("Enter size of matrix: ");
    scanf("%d", size);
    
    for (int i = 0; i < *size; i++)
        for (int j = 0; j < *size; j++)
            scanf("%lf", &m[i][j]);
    
    printf("Enter constant terms: ");
    for (int i = 0; i < *size; i++)
        scanf("%lf", &b[i]);
}

void copyMatrix (matrix a, matrix b, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            a[i][j] = b[i][j];
}

void printMatrix (matrix m, int size) {
    for (int n = 0; n < size; n++) {
        printf("\n");
        for (int k = 0; k < size; k++)
            printf("%12.5lf", m[n][k]);
    }
    printf("\n");
}

void printVector (vector v, int size) {
    printf("( ");
    for (int i = 0; i < size; i++) {
        printf("%lf  ", v[i]);
    }
    printf(")\n");
}

void multiplyMatrixVector (matrix m, vector v, int size, vector res) {
    for (int z = 0; z < size; z++) {
        res[z] = 0;
        
        for (int l = 0; l < size; l++)
            res[z] += m[z][l] * v[l];
    }
}

void subVectors (vector a, vector b, int size, vector res) {
    for (int k = 0; k < size; k++)
        res[k] = a[k] - b[k];
}

void rookPivoting (matrix m, int *i, int *j, int size) {
    
    int I_0 = *i, J_0 = *j;
    
    double max = m[row[I_0]][col[J_0]];
    int flag = 1;
    
    while (flag) {
        flag = 0;
        for (int I = I_0; I < size; I++) {
            if (fabs(m[row[I]][col[*j]]) > max) {
                max = fabs(m[row[I]][col[*j]]);
                *i = I;
                flag = 1;
            }
        }
        for (int J = J_0; J < size; J++) {
            if (fabs(m[row[*i]][col[J]]) > max) {
                max = fabs(m[row[*i]][col[J]]);
                *j = J;
                flag = 1;
            }
        }
    }
}

void swap (int *arr, int i, int j) {
    if (i != j) transpositions++;
    int n = arr[i];
    arr[i] = arr[j];
    arr[j] = n;
}

void LUdecomposition (matrix A, matrix LU, int size, void (*pivot)(matrix, int *, int *, int)) {
    int m = 0 , n = 0;
    double r;
    
    copyMatrix(LU, A, size);
    
    for (int i = 0; i < size; i++) {
        row[i] = i;
        col[i] = i;
    }
    
    for (int i = 0; i < size - 1; i++) {
        m = i;
        n = i;
        pivot(LU, &m, &n, size);
        swap(row, i, m);
        swap(col, i, n);
        for (int k = i + 1; k < size; k++) {
            
            r = LU[row[k]][col[i]] / LU[row[i]][col[i]];
            LU[row[k]][col[i]] = r;
            
            for (int l = i + 1; l < size; l++)
                LU[row[k]][col[l]] = LU[row[k]][col[l]] - r * LU[row[i]][col[l]];
        }
    }
}

void gauss (matrix LU, vector x, vector b, int size) {
    vector y;
    double sum;
    
    for (int i = 0; i < size; i++) {
        sum = 0;
        for (int j = 0; j < i; j++)
            sum = sum + LU[row[i]][col[j]] * y[col[j]];
        y[col[i]] = b[row[i]] - sum;
    }
    
    for (int i = size - 1; i >= 0; i--) {
        sum = 0;
        for (int k = i + 1; k < size; k++)
            sum = sum + LU[row[i]][col[k]] * x[col[k]];
        x[col[i]] = (y[col[i]] - sum) / LU[row[i]][col[i]];
    }
}

int GaussianElimination (matrix A, vector x, vector b, int size, void (*pivot)(matrix, int *, int *, int)) {
    matrix LU;
    transpositions = 0;
    
    for (int i = 0; i < size; i++) {
        row[i] = i;
        col[i] = i;
    }
    
    LUdecomposition(A, LU, size, pivot);
    
    if (LU[row[size - 1]][col[size - 1]] == 0)
        return 0;
    
    gauss(LU, x, b, size);
    return 1;
}

void iterativeRefinement (matrix A, vector x, vector b, int size, void (*pivot)(matrix, int *, int *, int)) {
    vector r, x1, dx;
    multiplyMatrixVector(A, x, size, x1);
    subVectors(b, x1, size, r);
    
    GaussianElimination(A, dx, r, size, pivot);
    
    for (int i = 0; i < size; i++)
        x[i] = x[i] + dx[i];
}

void inverseMatrix (matrix A, matrix A1, int size) {
    vector x, k;
    
    for (int i = 0; i < size; i++) {
        for (int n = 0; n < size; n++)
            k[n] = 0;
        k[i] = 1;
        GaussianElimination(A, x, k, size, rookPivoting);
        for (int j = 0; j < size; j++)
            A1[j][i] = x[j];
    }
}

double getVectorNorm(int ind, vector v, int size) {
    double sum = 0;
    double max = v[0];
    
    switch (ind) {
        case 1:
            for (int i = 0; i < size; i++)
                sum += fabs(v[i]);
            return sum;
        case 2:
            for (int i = 0; i < size; i++)
                if (fabs(v[i]) > max)
                    max = fabs(v[i]);
            return max;
        case 3:
            for (int i = 0; i < size; i++)
                sum += v[i] * v[i];
            return sqrt(sum);
        default: return 0;
    }
}

double getResidualNorm (int id, matrix A, vector x, vector b, int size) {
    vector v1, v2;
    multiplyMatrixVector(A, x, size, v1);
    subVectors(v1, b, size, v2);
    return getVectorNorm(id, v2, size);
}

void multiplyMatrix (matrix a, matrix b, int size, matrix ab) {
    double sum;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            sum = 0;
            for (int k = 0; k < size; k++)
                sum += a[i][k] * b[k][j];
            ab[i][j] = sum;
        }
    }
}

double getRelativeResidualNorm (int id, matrix A, vector x, vector b, int size) {
    return getResidualNorm (id, A, x, b, size) / getVectorNorm(id, b, size);
}

double relativeError (int id, vector x, vector y, int size) {
    vector sub;
    subVectors(x, y, size, sub);
    return getVectorNorm(id, sub, size)/ getVectorNorm(id, y, size);
}

double matrixDeterminant (matrix A, int size) {
    matrix LU;
    LUdecomposition(A, LU, size, rookPivoting);
    double det = 1;
    
    for (int i = 0; i < size; i++)
        det *= LU[row[i]][col[i]];
    
    if (transpositions % 2)
        det = - det;
    
    return det;
}