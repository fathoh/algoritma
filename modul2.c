#include <stdio.h>
#include <stdlib.h>

// Alokasi matriks
int** alloc_matrix(int n) {
    int** m = (int**) malloc(n * sizeof(int*));
    if (!m) return NULL;

    for (int i = 0; i < n; i++) {
        m[i] = (int*) malloc(n * sizeof(int));
        if (!m[i]) {
            for (int j = 0; j < i; j++)
                free(m[j]);
            free(m);
            return NULL;
        }
    }
    return m;
}

// Bebaskan memori matriks
void free_matrix(int** m, int n) {
    if (!m) return;
    for (int i = 0; i < n; i++)
        free(m[i]);
    free(m);
}

// Penjumlahan matriks
void add(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

// Pengurangan matriks
void sub(int** A, int** B, int** C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}

// Strassen rekursif
void strassen(int** A, int** B, int** C, int n) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int k = n / 2;

    int **A11 = alloc_matrix(k), **A12 = alloc_matrix(k),
        **A21 = alloc_matrix(k), **A22 = alloc_matrix(k);
    int **B11 = alloc_matrix(k), **B12 = alloc_matrix(k),
        **B21 = alloc_matrix(k), **B22 = alloc_matrix(k);

    int **M1 = alloc_matrix(k), **M2 = alloc_matrix(k), **M3 = alloc_matrix(k),
        **M4 = alloc_matrix(k), **M5 = alloc_matrix(k), **M6 = alloc_matrix(k),
        **M7 = alloc_matrix(k);
    int **T1 = alloc_matrix(k), **T2 = alloc_matrix(k);

    if (!A11 || !A12 || !A21 || !A22 || !B11 || !B12 || !B21 || !B22 ||
        !M1 || !M2 || !M3 || !M4 || !M5 || !M6 || !M7 || !T1 || !T2) {
        free_matrix(A11, k); free_matrix(A12, k); free_matrix(A21, k); free_matrix(A22, k);
        free_matrix(B11, k); free_matrix(B12, k); free_matrix(B21, k); free_matrix(B22, k);
        free_matrix(M1, k); free_matrix(M2, k); free_matrix(M3, k); free_matrix(M4, k);
        free_matrix(M5, k); free_matrix(M6, k); free_matrix(M7, k);
        free_matrix(T1, k); free_matrix(T2, k);
        return;
    }

    // Split matriks
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    add(A11, A22, T1, k); add(B11, B22, T2, k);
    strassen(T1, T2, M1, k);

    add(A21, A22, T1, k);
    strassen(T1, B11, M2, k);

    sub(B12, B22, T2, k);
    strassen(A11, T2, M3, k);

    sub(B21, B11, T2, k);
    strassen(A22, T2, M4, k);

    add(A11, A12, T1, k);
    strassen(T1, B22, M5, k);

    sub(A21, A11, T1, k);
    add(B11, B12, T2, k);
    strassen(T1, T2, M6, k);

    sub(A12, A22, T1, k);
    add(B21, B22, T2, k);
    strassen(T1, T2, M7, k);

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            C[i][j + k] = M3[i][j] + M5[i][j];
            C[i + k][j] = M2[i][j] + M4[i][j];
            C[i + k][j + k] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
    }

    free_matrix(A11, k); free_matrix(A12, k); free_matrix(A21, k); free_matrix(A22, k);
    free_matrix(B11, k); free_matrix(B12, k); free_matrix(B21, k); free_matrix(B22, k);
    free_matrix(M1, k); free_matrix(M2, k); free_matrix(M3, k); free_matrix(M4, k);
    free_matrix(M5, k); free_matrix(M6, k); free_matrix(M7, k);
    free_matrix(T1, k); free_matrix(T2, k);
}

int main() {
    int n = 2;

    int** A = alloc_matrix(n);
    int** B = alloc_matrix(n);
    int** C = alloc_matrix(n);

    if (!A || !B || !C) {
        fprintf(stderr, "Gagal alokasi matriks\n");
        free_matrix(A, n); free_matrix(B, n); free_matrix(C, n);
        return 1;
    }

    A[0][0] = 1; A[0][1] = 2;
    A[1][0] = 3; A[1][1] = 4;

    B[0][0] = 5; B[0][1] = 6;
    B[1][0] = 7; B[1][1] = 8;

    strassen(A, B, C, n);

    printf("Hasil:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", C[i][j]);
        printf("\n");
    }

    free_matrix(A, n);
    free_matrix(B, n);
    free_matrix(C, n);
    return 0;
}
