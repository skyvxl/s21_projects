#include "../s21_matrix.h"

int calculate_complements(matrix_t *A, matrix_t *result) {
  int status = OK;
  matrix_t temp;

  if (s21_create_matrix(A->rows - 1, A->columns - 1, &temp) != OK ||
      s21_create_matrix(A->rows, A->columns, result) != OK) {
    s21_remove_matrix(result);
    s21_remove_matrix(&temp);
    status = INCORRECT_MATRIX;
  } else {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        fill_minor(&temp, i, j, A);
        s21_determinant(&temp, &result->matrix[i][j]);
        result->matrix[i][j] *= ((i + j) % 2 == 0 ? 1 : -1);
      }
    }
    s21_remove_matrix(&temp);
  }

  return status;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  int status = OK;

  if (is_matrix_incorrect(A) || !result) {
    status = INCORRECT_MATRIX;
  } else if (!is_matrix_square(A)) {
    status = CALCULATION_ERR;
  } else {
    if (A->rows > 1) {
      status = calculate_complements(A, result);
    } else {
      s21_create_matrix(A->rows, A->columns, result);
      result->matrix[0][0] = 1;
    }
  }

  return status;
}