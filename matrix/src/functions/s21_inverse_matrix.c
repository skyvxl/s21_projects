#include "../s21_matrix.h"

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (is_matrix_incorrect(A) || !result) {
    return INCORRECT_MATRIX;
  }
  int status = OK;

  if (!is_matrix_square(A)) status = CALCULATION_ERR;

  if (status == OK) {
    double determinant = 0;
    s21_determinant(A, &determinant);

    if (is_almost_zero(determinant)) {
      status = CALCULATION_ERR;
    } else {
      status = inverse_matrix(A, determinant, result);
    }
  }

  return status;
}