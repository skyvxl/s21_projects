#include "../s21_matrix.h"

int s21_transpose(matrix_t *A, matrix_t *result) {
  if (!result || is_matrix_incorrect(A)) {
    return INCORRECT_MATRIX;
  }

  int status = OK;

  if (A != result) {
    status = s21_create_matrix(A->columns, A->rows, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[j][i] = A->matrix[i][j];
      }
    }
  } else {
    double temp;
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns && i != j; j++) {
        temp = A->matrix[i][j];
        A->matrix[i][j] = result->matrix[j][i];
        result->matrix[j][i] = temp;
      }
    }
  }
  return status;
}
