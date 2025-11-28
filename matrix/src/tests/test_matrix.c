#include "matrix_test.h"

int main() {
  int failed = 0;
  Suite *matrix_test[] = {test_s21_create_matrix(),
                          test_s21_eq_matrix(),
                          test_s21_sum_matrix(),
                          test_s21_sub_matrix(),
                          test_s21_mult_number(),
                          test_s21_mult_matrix(),
                          test_s21_transpose(),
                          test_s21_determinant(),
                          test_s21_calc_complements(),
                          test_s21_inverse_matrix(),
                          NULL};

  for (int i = 0; matrix_test[i] != NULL; i++) {
    SRunner *sr = srunner_create(matrix_test[i]);
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);
    failed += srunner_ntests_failed(sr);
    srunner_free(sr);
  }
  if (failed == 0) {
    printf("\033[1;32mAll tests passed\033[0m\n");
  } else {
    printf("\033[1;31mFailed tests: %d\033[0m\n", failed);
  }
  return (failed == 0) ? 0 : 1;
}

void init_matrix(matrix_t *Matrix, double const *values, size_t length) {
  int c = 0;
  for (int i = 0; i < Matrix->rows; i++) {
    for (int j = 0; j < Matrix->columns; j++) {
      if (c < (int)length)
        Matrix->matrix[i][j] = values[c++];
      else
        Matrix->matrix[i][j] = 0;
    }
  }
}