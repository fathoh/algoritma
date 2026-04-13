#include <stdio.h>
#include <stdlib.h>

// Alokasi matriks
int** alloc_matrix(int n) {
    int** m = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        m[i] = (int*) malloc(n * sizeof(int));
    }
    return m;
}

// Membebaskan memori
void free_matrix(int** m, int n) {
    for (int i = 0; i < n; i++) {
        free(m[i]);
    }
    free(m);
}

int main() {
    int n = 2;

    int** A = alloc_matrix(n);
    int** B = alloc_matrix(n);
    int** C = alloc_matrix(n);

    // Isi matriks A
    A[0][0] = 1; A[0][1] = 2;
    A[1][0] = 3; A[1][1] = 4;

    // Isi matriks B
    B[0][0] = 5; B[0][1] = 6;
    B[1][0] = 7; B[1][1] = 8;

    // Perkalian matriks (3 loop)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Output
    printf("Hasil Perkalian:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    // Free memori
    free_matrix(A, n);
    free_matrix(B, n);
    free_matrix(C, n);

    return 0;
}
