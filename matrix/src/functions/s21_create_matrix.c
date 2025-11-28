#include "../s21_matrix.h"

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (rows <= 0 || columns <= 0 || !result) return INCORRECT_MATRIX;

  int status = OK;

  result->rows = rows;
  result->columns = columns;
  if (my_create_matrix(rows, columns, &result->matrix) != OK) {
    result->matrix = NULL;
    status = INCORRECT_MATRIX;
  }

  return status;
}
