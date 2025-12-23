#include <stdio.h>
#include <stdlib.h>

int main() {
    int rows, cols;

    printf("Количество строк: ");
    scanf("%d", &rows);
    printf("Количество столбцов: ");
    scanf("%d", &cols);

    int** matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols* sizeof(int));
    }

    printf("Элементы матрицы:\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

    int* vector = (int*)malloc(cols* sizeof(int));
    printf("Элементы вектора:\n", cols);
    for (int i = 0; i < cols; i++) {
        scanf("%d", &vector[i]);
    }
    int* result = (int*)malloc(rows * sizeof(int));

    for (int i = 0; i < rows; i++) {
        result[i] = 0;
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    printf("Итог:\n");
    for (int i = 0; i < rows; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");

    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(result);

    return 0;
}