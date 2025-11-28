#include "../s21_matrix.h"

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  if (is_matrix_incorrect(A) || is_matrix_incorrect(B)) return FAILURE;
  if (A->rows != B->rows || A->columns != B->columns) return FAILURE;

  int result = SUCCESS;

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      if (fabs(A->matrix[i][j] - B->matrix[i][j]) > 1e-7) {
        result = FAILURE;
        break;
      }
    }
    if (result == FAILURE) break;
  }

  return result;
}