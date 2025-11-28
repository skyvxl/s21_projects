#include "matrix_test.h"

START_TEST(eq_matrix_test1) {
  ///////////////////////////////
  matrix_t A = {0, 0, 0};
  matrix_t B = {0, 0, 0};
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
}
END_TEST

START_TEST(eq_matrix_test2) {
  ///////////////////////////////
  matrix_t *A = NULL;
  matrix_t B = {0, 0, 0};
  ck_assert_int_eq(s21_eq_matrix(A, &B), FAILURE);
}
END_TEST

START_TEST(eq_matrix_testNULL) {
  ///////////////////////////////
  matrix_t *B = NULL;
  matrix_t A = {0, 0, 0};
  s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(s21_eq_matrix(&A, B), FAILURE);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(eq_matrix_test3) {
  ///////////////////////////////
  matrix_t A;
  matrix_t B;
  s21_create_matrix(4, 4, &A);
  s21_create_matrix(4, 4, &B);
  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_test4) {
  ///////////////////////////////
  matrix_t A;
  matrix_t B;
  s21_create_matrix(4, 4, &A);
  s21_create_matrix(4, 4, &B);
  double values[16] = {
      0.1, 0.12, 0.123, 0.1234, 0.12345, 0.123456, 0.1234567,
  };
  init_matrix(&A, values, 16);
  init_matrix(&B, values, 16);
  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(eq_matrix_test5) {
  ///////////////////////////////
  matrix_t A;
  matrix_t B;
  s21_create_matrix(4, 4, &A);
  s21_create_matrix(4, 4, &B);
  double values_1[16] = {
      0.1, 0.12, 0.123, 0.1234, 0.12345, 0.123456, 0.1234567,
  };
  double values_2[16] = {
      0.1, 0.123, 0.123, 0.1234, 0.12345, 0.123456, 0.1234567,
  };
  init_matrix(&A, values_1, 16);
  init_matrix(&B, values_2, 16);
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

Suite *test_s21_eq_matrix(void) {
  Suite *s = suite_create("\033[45m S21_eq_matrix \033[0m");
  TCase *tc_eq_matrix = tcase_create("tc_eq_matrix");

  tcase_add_test(tc_eq_matrix, eq_matrix_test1);
  tcase_add_test(tc_eq_matrix, eq_matrix_test2);
  tcase_add_test(tc_eq_matrix, eq_matrix_test3);
  tcase_add_test(tc_eq_matrix, eq_matrix_test4);
  tcase_add_test(tc_eq_matrix, eq_matrix_test5);
  tcase_add_test(tc_eq_matrix, eq_matrix_testNULL);

  suite_add_tcase(s, tc_eq_matrix);
  return s;
}