#include "../s21_matrix.h"

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  if (is_matrix_incorrect(A) || !result) return INCORRECT_MATRIX;

  int status = copy_matrix(A, result);

  if (status == OK) {
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] *= number;
      }
    }
  }

  return status;
}