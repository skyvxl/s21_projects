#include "../s21_matrix.h"

int s21_determinant(matrix_t *A, double *result) {
  if (is_matrix_incorrect(A) || !result) return INCORRECT_MATRIX;

  int status = OK;

  if (!is_matrix_square(A)) status = CALCULATION_ERR;

  if (status == OK) {
    if (A->rows == 1) {
      *result = A->matrix[0][0];
    } else if (A->rows == 2) {
      *result =
          A->matrix[0][0] * A->matrix[1][1] - A->matrix[1][0] * A->matrix[0][1];
    } else {
      *result = 0;
      matrix_t minor;
      if (s21_create_matrix(A->rows - 1, A->rows - 1, &minor) != OK) {
        status = INCORRECT_MATRIX;
      }

      if (status == OK) {
        int sign = 1;
        for (int i = 0; i < A->rows; i++) {
          double temp_result;
          fill_minor(&minor, 0, i, A);
          status = s21_determinant(&minor, &temp_result);
          *result += sign * A->matrix[0][i] * temp_result;
          sign *= -1;
        }
        s21_remove_matrix(&minor);
      }
    }
  }

  return status;
}