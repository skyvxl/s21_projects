#include "../s21_matrix.h"

int my_create_matrix(int rows, int columns, double ***matrix) {
  *matrix = (double **)calloc(rows, sizeof(double *));
  if (!*matrix) return INCORRECT_MATRIX;

  for (int i = 0; i < rows; i++) {
    (*matrix)[i] = (double *)calloc(columns, sizeof(double));
    if (!(*matrix)[i]) {
      for (int j = 0; j < i; j++) {
        free((*matrix)[j]);
      }
      free(*matrix);
      return INCORRECT_MATRIX;
    }
  }

  return OK;
}

void destroy(double **matrix, int rows) {
  for (int i = 0; i < rows; i++) {
    if (matrix[i]) {
      free(matrix[i]);
      matrix[i] = NULL;
    }
  }
  free(matrix);
  matrix = NULL;
}

int is_matrix_incorrect(matrix_t *A) {
  return !A || A->rows < 1 || A->columns < 1 || !A->matrix;
}

int copy_matrix(matrix_t *A, matrix_t *result) {
  if (A == result) return OK;

  int status = s21_create_matrix(A->rows, A->columns, result);
  if (status == OK) {
    result->columns = A->columns;
    result->rows = A->rows;
    for (int i = 0; i < result->rows; i++) {
      for (int j = 0; j < result->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j];
      }
    }
  }
  return status;
}

int is_matrix_correct_for_mul(matrix_t *A, matrix_t *B) {
  return A && B && A->matrix && B->matrix && A->columns == B->rows;
}

void mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->columns; j++) {
      result->matrix[i][j] = 0;
      for (int k = 0; k < A->columns; k++) {
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }
}

void fill_minor(matrix_t *minor, int rows, int columns, matrix_t *A) {
  if (!minor) return;

  int minor_row = 0, minor_col = 0;

  for (int i = 0; i < A->rows; i++) {
    if (i == rows) continue;

    minor_col = 0;
    for (int j = 0; j < A->columns; j++) {
      if (j == columns) continue;

      minor->matrix[minor_row][minor_col] = A->matrix[i][j];
      minor_col++;
    }
    minor_row++;
  }
}

int is_matrix_square(matrix_t *A) { return A && A->rows == A->columns; }

int is_almost_zero(double value) { return fabs(value) < FLT_EPSILON; }

int inverse_matrix(matrix_t *A, double determinant, matrix_t *result) {
  int status = OK;

  matrix_t complement_matrix;
  if (s21_calc_complements(A, &complement_matrix) == OK) {
    s21_transpose(&complement_matrix, &complement_matrix);
    status = s21_mult_number(&complement_matrix, 1 / determinant, result);
    s21_remove_matrix(&complement_matrix);
  } else {
    status = INCORRECT_MATRIX;
  }

  return status;
}
