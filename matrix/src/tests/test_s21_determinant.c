#include "matrix_test.h"

START_TEST(determinant_test1) {
  ///////////////////////////////
  matrix_t A = {0, 0, 0};
  double result;
  ck_assert_int_eq(s21_determinant(&A, &result), INCORRECT_MATRIX);
}
END_TEST

START_TEST(determinant_test2) {
  ///////////////////////////////
  matrix_t A;
  double result;

  s21_create_matrix(1, 1, &A);
  double values_A[] = {5};
  init_matrix(&A, values_A, 1);

  ck_assert_int_eq(s21_determinant(&A, &result), OK);
  ck_assert_double_eq(result, 5);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_test3) {
  ///////////////////////////////
  matrix_t A;
  double result;

  s21_create_matrix(2, 2, &A);
  double values_A[] = {1, 2, 3, 4};
  init_matrix(&A, values_A, 4);

  ck_assert_int_eq(s21_determinant(&A, &result), OK);
  ck_assert_double_eq(result, -2);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_test4) {
  ///////////////////////////////
  matrix_t A;
  double result;

  s21_create_matrix(3, 3, &A);
  double values_A[] = {1, 2, 3, 0, 1, 4, 5, 6, 0};
  init_matrix(&A, values_A, 9);

  ck_assert_int_eq(s21_determinant(&A, &result), OK);
  ck_assert_double_eq(result, 1);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_test5) {
  ///////////////////////////////
  matrix_t A;
  double result;

  s21_create_matrix(3, 3, &A);
  double values_A[] = {2, -3, 1, 2, 0, -1, 1, 4, 5};
  init_matrix(&A, values_A, 9);

  ck_assert_int_eq(s21_determinant(&A, &result), OK);
  ck_assert_double_eq(result, 49);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_test6) {
  ///////////////////////////////
  matrix_t A;
  double result;

  s21_create_matrix(4, 4, &A);
  double values_A[] = {1, 0, 2, -1, 3, 0, 0, 5, 2, 1, 4, -3, 1, 0, 5, 0};
  init_matrix(&A, values_A, 16);

  ck_assert_int_eq(s21_determinant(&A, &result), OK);
  ck_assert_double_eq(result, 30);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_fp_test1) {
  ///////////////////////////////
  matrix_t A;
  double result;

  s21_create_matrix(2, 2, &A);
  double values_A[] = {1.1, 2.2, 3.3, 4.4};
  init_matrix(&A, values_A, 4);

  ck_assert_int_eq(s21_determinant(&A, &result), OK);
  ck_assert_double_eq_tol(result, -2.42, 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_fp_test2) {
  ///////////////////////////////
  matrix_t A;
  double result;

  s21_create_matrix(3, 3, &A);
  double values_A[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9};
  init_matrix(&A, values_A, 9);

  ck_assert_int_eq(s21_determinant(&A, &result), OK);
  ck_assert_double_eq_tol(result, 0, 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_fp_test3) {
  ///////////////////////////////
  matrix_t A;
  double result;

  s21_create_matrix(3, 3, &A);
  double values_A[] = {1.1, 0.0, 0.0, 0.0, 1.1, 0.0, 0.0, 0.0, 1.1};
  init_matrix(&A, values_A, 9);

  ck_assert_int_eq(s21_determinant(&A, &result), OK);
  ck_assert_double_eq_tol(result, 1.331, 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_fp_test4) {
  ///////////////////////////////
  matrix_t A;
  double result;

  s21_create_matrix(4, 4, &A);
  double values_A[] = {1.1, 0.0, 2.2, -1.1, 3.3, 0.0, 0.0, 5.5,
                       2.2, 1.1, 4.4, -3.3, 1.1, 0.0, 5.5, 0.0};
  init_matrix(&A, values_A, 16);

  ck_assert_int_eq(s21_determinant(&A, &result), OK);
  ck_assert_double_eq_tol(result, 43.923, 1e-7);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(determinant_test7) {
  ///////////////////////////////
  matrix_t A;
  double result;

  s21_create_matrix(4, 3, &A);
  double values_A[] = {1.1, 0.0, 2.2, -1.1, 3.3, 0.0, 0.0, 5.5,
                       2.2, 1.1, 4.4, -3.3, 1.1, 0.0, 5.5, 0.0};
  init_matrix(&A, values_A, 12);

  ck_assert_int_eq(s21_determinant(&A, &result), CALCULATION_ERR);

  s21_remove_matrix(&A);
}
END_TEST

Suite *test_s21_determinant(void) {
  Suite *s = suite_create("\033[45m S21_determinant \033[0m");
  TCase *tc_determinant = tcase_create("tc_determinant");

  tcase_add_test(tc_determinant, determinant_test1);
  tcase_add_test(tc_determinant, determinant_test2);
  tcase_add_test(tc_determinant, determinant_test3);
  tcase_add_test(tc_determinant, determinant_test4);
  tcase_add_test(tc_determinant, determinant_test5);
  tcase_add_test(tc_determinant, determinant_test6);
  tcase_add_test(tc_determinant, determinant_test7);

  // Добавленные тесты для значений с плавающей точкой
  tcase_add_test(tc_determinant, determinant_fp_test1);
  tcase_add_test(tc_determinant, determinant_fp_test2);
  tcase_add_test(tc_determinant, determinant_fp_test3);
  tcase_add_test(tc_determinant, determinant_fp_test4);

  suite_add_tcase(s, tc_determinant);
  return s;
}