#include "../s21_matrix.h"

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (is_matrix_incorrect(A) || is_matrix_incorrect(B) || !result)
    return INCORRECT_MATRIX;
  else if (!(A->rows == B->rows) || !(A->columns == B->columns))
    return CALCULATION_ERR;

  int status = OK;

  if (status == OK) {
    status = copy_matrix(A, result);
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] -= B->matrix[i][j];
      }
    }
  }

  return status;
}