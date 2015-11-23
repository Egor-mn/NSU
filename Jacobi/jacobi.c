//
//  jacobi.c
//  Jacobi
//
//  Created by Егор Манылов on 14.10.15.
//  Copyright © 2015 Egor_Mn. All rights reserved.
//

#include "jacobi.h"

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

void hilbertMatrix(matrix m, int *size) {
    printf("Enter size of matrix: ");
    scanf("%d", size);

    for (int i = 0; i < *size; i++)
        for (int j = 0; j < *size; j++)
            m[i][j] = 1.0 / (i + j + 1);
}

void keybordMatrix(matrix m, int *size) {
    printf("Enter size of matrix: ");
    scanf("%d", size);

    for (int i = 0; i < *size; i++)
        for (int j = 0; j < *size; j++)
            scanf("%lf", &m[i][j]);
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

void copyMatrix (matrix a, matrix b, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            a[i][j] = b[i][j];
}

void printMatrix(matrix m, int size) {
    for (int n = 0; n < size; n++) {
        printf("\n");
        for (int k = 0; k < size; k++)
            printf("%12.5lf", m[n][k]);

    }
    printf("\n");
}

void printDiagonal(matrix m, int size) {
    for (int i = 0; i < size; i++)
        printf("%3d: %.30e\n", i + 1, m[i][i]);
}

void multiplyRotationMatrix(matrix a, matrix eigen_vectors, int i, int j, int size) {
    double C = a[i][j];
    double F = (a[j][j] - a[i][i])/2;
    double t;

    if (F > 0) t = -C/(F + sqrt(F*F + C*C));
    else t =  -C/(F - sqrt(F*F + C*C));

    double c = 1/sqrt(1 + t*t);
    double s = t * c;

    matrix b, sv;
    copyMatrix(b, a, size);

    for (int l = 0; l < size; l++) {
        b[i][l] = s * a[j][l] + c * a[i][l];
        b[j][l] = c * a[j][l] - s * a[i][l];
    }

    copyMatrix(a, b, size);
    for (int l = 0; l < size; l++) {
        a[l][i] = s * b[l][j] + c * b[l][i];
        a[l][j] = c * b[l][j] - s * b[l][i];
    }

    copyMatrix(sv, eigen_vectors, size);
    for (int l = 0; l < size; l++) {
        sv[l][i] = s * eigen_vectors[l][j] + c * eigen_vectors[l][i];
        sv[l][j] = c * eigen_vectors[l][j] - s * eigen_vectors[l][i];
    }
    copyMatrix(eigen_vectors, sv, size);
}

void multiplyVectorConst(vector a, double c, int size) {
    for (int k = 0; k < size; k++)
        a[k] = a[k] * c;
}

void multiplyMatrixVector(matrix m, vector v, int size, vector res) {
    for (int z = 0; z < size; z++) {
        res[z] = 0;

        for (int l = 0; l < size; l++)
            res[z] += m[z][l] * v[l];
    }
}

void subVectors(vector a, vector b, int size, vector res) {
    for (int k = 0; k < size; k++)
        res[k] = a[k] - b[k];
}

double getDotProduct(vector a, vector b, int size) {
    double sum = 0;

    for (int i = 0; i < size; i++)
        sum += a[i] * b[i];

    return sum;
}

double getMaxOffDiagonal(matrix m, int size, int *x, int *y) {
    double max = 0;
    for (int j = 1; j < size; j++)
        for (int i = 0; i < j; i++)
            if (max < fabs(m[i][j])) {
                max = fabs(m[i][j]);
                *x = i;
                *y = j;
            }
    
    
    return max;
}

double getAverageOffDiagonal(matrix m, int size) {
    double sum = 0;
    for (int j = 1; j < size; j++)
        for (int i = 0; i < j; i++)
            sum += fabs(m[i][j]);

    return sum / size;
}

double getTraceMatrix(matrix m, int size) {
    double trace = 0;
    for (int i = 0; i < size; i++)
        trace +=  m[i][i];

    return trace;
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

double getResidualNorm(int id, matrix a, vector x, vector y, double c, int size) {
    vector v1, v2;
    multiplyMatrixVector(a, x, size, v1);
    multiplyVectorConst(y, c, size);
    subVectors(v1, y, size, v2);
    return getVectorNorm(id, v2, size);
}

double getRayleighQuotient(int id, matrix a, matrix eigen_vectors, int size) {
    vector v1, e;
    for (int i = 0; i < size; i++)
        e[i] = eigen_vectors[i][id];

    multiplyMatrixVector(a, e, size, v1);
    double n = getDotProduct(e, v1, size);
    double d = getDotProduct(e, e, size);

    return n / d;
}

int roundRobin(matrix m, matrix eigen_vectors, int size) {
    int iteration = 0;
    int i, j;
    while (getMaxOffDiagonal(m, size, &i, &j) > eps && iteration < max_iteration) {
        multiplyRotationMatrix(m, eigen_vectors, i, j, size);
        printf("\nIteration %d:", ++iteration);
        printf("\nMax off-diagonal element: %lf", getMaxOffDiagonal(m, size, &i, &j));
        printf("\nAverage off-diagonal element: %lf", getAverageOffDiagonal(m, size));
        printf("\nTrace of matrix: %lf\nMatrix", getTraceMatrix(m, size));
        printMatrix(m, size);
    }
    return iteration;
}

void analysisOfResults(matrix m, matrix orig, matrix eigen_vectors, int size) {
    int c = 0, n, id;
    double N;
    vector v, v2;

    while (c != 4) {
        printf("\nAnalysis of results: (1) residual vector, (2) Rayleigh quotient, (3) dot product of two eigenvectors (4) end? ");
        scanf("%d", &c);
        switch (c) {
            case 1:
                printf("Enter the number of Eigenvalues (1-matrix size) and number of norm (1-3): ");
                scanf("%d %d", &n, &id);
                for (int i = 0; i < size; i++)
                    v[i] = eigen_vectors[i][n-1];

                N = getResidualNorm(id, orig, v, v, m[n-1][n-1], size);
                printf("Norm of residual vector: %e\n", N);
                break;
            case 2:
                printf("Enter the number of eigenvector (1-matrix size): ");
                scanf("%d", &n);

                N = getRayleighQuotient(n, orig, eigen_vectors, size);
                printf("Rayleigh quotient: %e\n", N);
                break;
            case 3:
                printf("Enter two numbers of eigenvector (1-matrix size): ");
                scanf("%d %d", &n, &id);
                for (int i = 0; i < size; i++) {
                    v[i] = eigen_vectors[i][n-1];
                    v2[i] = eigen_vectors[i][id -1];
                }

                N = getDotProduct(v, v2, size);
                printf("Dot product: %e\n", N);
                break;
            default: return;
        }
    }
}
