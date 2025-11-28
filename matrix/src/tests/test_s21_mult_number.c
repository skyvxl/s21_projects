#include <float.h>
#include <limits.h>

#include "matrix_test.h"

START_TEST(mult_number_test2) {
  ///////////////////////////////
  // success creation
  matrix_t A, expected;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);
  double values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double values_ex[] = {5.2, 10.4, 15.6, 20.8, 26, 31.2, 36.4, 41.6, 46.8};
  init_matrix(&A, values, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_mult_number(&A, 5.2, &A), OK);
  ck_assert_int_eq(s21_eq_matrix(&A, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(mult_number_test_int) {
  matrix_t A, expected;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);
  double values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double values_ex[] = {2, 4, 6, 8, 10, 12, 14, 16, 18};
  init_matrix(&A, values, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_mult_number(&A, 2, &A), OK);
  ck_assert_int_eq(s21_eq_matrix(&A, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(mult_number_test_max_int) {
  matrix_t A, expected;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);
  double values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double values_ex[] = {1.0 * INT_MAX, 2.0 * INT_MAX, 3.0 * INT_MAX,
                        4.0 * INT_MAX, 5.0 * INT_MAX, 6.0 * INT_MAX,
                        7.0 * INT_MAX, 8.0 * INT_MAX, 9.0 * INT_MAX};
  init_matrix(&A, values, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_mult_number(&A, INT_MAX, &A), OK);
  ck_assert_int_eq(s21_eq_matrix(&A, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(mult_number_test_min_int) {
  matrix_t A, expected;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);
  double values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double values_ex[] = {1.0 * INT_MIN, 2.0 * INT_MIN, 3.0 * INT_MIN,
                        4.0 * INT_MIN, 5.0 * INT_MIN, 6.0 * INT_MIN,
                        7.0 * INT_MIN, 8.0 * INT_MIN, 9.0 * INT_MIN};
  init_matrix(&A, values, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_mult_number(&A, INT_MIN, &A), OK);
  ck_assert_int_eq(s21_eq_matrix(&A, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(mult_number_test_max_float) {
  matrix_t A, expected;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);
  double values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double values_ex[] = {1.0 * FLT_MAX, 2.0 * FLT_MAX, 3.0 * FLT_MAX,
                        4.0 * FLT_MAX, 5.0 * FLT_MAX, 6.0 * FLT_MAX,
                        7.0 * FLT_MAX, 8.0 * FLT_MAX, 9.0 * FLT_MAX};
  init_matrix(&A, values, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_mult_number(&A, FLT_MAX, &A), OK);
  ck_assert_int_eq(s21_eq_matrix(&A, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(mult_number_test_min_float) {
  matrix_t A, expected;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);
  double values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double values_ex[] = {1.0 * FLT_MIN, 2.0 * FLT_MIN, 3.0 * FLT_MIN,
                        4.0 * FLT_MIN, 5.0 * FLT_MIN, 6.0 * FLT_MIN,
                        7.0 * FLT_MIN, 8.0 * FLT_MIN, 9.0 * FLT_MIN};
  init_matrix(&A, values, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_mult_number(&A, FLT_MIN, &A), OK);
  ck_assert_int_eq(s21_eq_matrix(&A, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(mult_number_test_negative) {
  matrix_t A, expected;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);
  double values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double values_ex[] = {-1, -2, -3, -4, -5, -6, -7, -8, -9};
  init_matrix(&A, values, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_mult_number(&A, -1, &A), OK);
  ck_assert_int_eq(s21_eq_matrix(&A, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(mult_number_test_zero) {
  matrix_t A, expected;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);
  double values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double values_ex[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  init_matrix(&A, values, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_mult_number(&A, 0, &A), OK);
  ck_assert_int_eq(s21_eq_matrix(&A, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
}
END_TEST

START_TEST(mult_number_testNULL) {
  ///////////////////////////////

  matrix_t *A = NULL;
  matrix_t *result = NULL;

  ck_assert_int_eq(s21_mult_number(A, 5.2, result), INCORRECT_MATRIX);
}
END_TEST

Suite *test_s21_mult_number(void) {
  Suite *s = suite_create("\033[45m S21_mult_number \033[0m");
  TCase *tc_mult_number = tcase_create("tc_mult_number");

  tcase_add_test(tc_mult_number, mult_number_test2);
  tcase_add_test(tc_mult_number, mult_number_test_int);
  tcase_add_test(tc_mult_number, mult_number_test_max_int);
  tcase_add_test(tc_mult_number, mult_number_test_min_int);
  tcase_add_test(tc_mult_number, mult_number_test_max_float);
  tcase_add_test(tc_mult_number, mult_number_test_min_float);
  tcase_add_test(tc_mult_number, mult_number_test_negative);
  tcase_add_test(tc_mult_number, mult_number_test_zero);
  tcase_add_test(tc_mult_number, mult_number_testNULL);

  suite_add_tcase(s, tc_mult_number);
  return s;
}