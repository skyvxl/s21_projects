#include "../s21_matrix.h"

int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (!result || is_matrix_incorrect(A) || is_matrix_incorrect(B))
    return INCORRECT_MATRIX;
  int status = OK;
  if (!is_matrix_correct_for_mul(A, B)) status = CALCULATION_ERR;

  if (status == OK) {
    status = s21_create_matrix(A->rows, B->columns, result);

    if (status == OK) {
      mult_matrix(A, B, result);
    }
  }

  return status;
}