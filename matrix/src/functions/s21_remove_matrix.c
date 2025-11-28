#include "../s21_matrix.h"

void s21_remove_matrix(matrix_t *A) {
  if (A && A->matrix) {
    destroy(A->matrix, A->rows);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}
