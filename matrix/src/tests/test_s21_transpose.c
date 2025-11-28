#include "matrix_test.h"

START_TEST(transpose_test1) {
  ///////////////////////////////
  matrix_t A = {0, 0, 0};
  matrix_t B = {0, 0, 0};
  ck_assert_int_eq(s21_transpose(&A, &B), INCORRECT_MATRIX);
}
END_TEST

START_TEST(transpose_test2) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(3, 2, &A);
  s21_create_matrix(2, 3, &expected);

  double values_A[] = {0, 1, 2, 3, 4, 5};
  double values_ex[] = {0, 3, 1, 4, 2, 5};

  init_matrix(&A, values_A, 6);
  init_matrix(&A, values_ex, 6);

  s21_transpose(&A, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &expected), OK);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(transpose_test3) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &expected);

  double values_A[] = {1, 2, 3, 4};
  double values_ex[] = {1, 3, 2, 4};

  init_matrix(&A, values_A, 4);
  init_matrix(&expected, values_ex, 4);

  s21_transpose(&A, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(transpose_test4) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(1, 3, &A);
  s21_create_matrix(3, 1, &expected);

  double values_A[] = {1, 2, 3};
  double values_ex[] = {1, 2, 3};

  init_matrix(&A, values_A, 3);
  init_matrix(&expected, values_ex, 3);

  s21_transpose(&A, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(transpose_test5) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(3, 1, &A);
  s21_create_matrix(1, 3, &expected);

  double values_A[] = {1, 2, 3};
  double values_ex[] = {1, 2, 3};

  init_matrix(&A, values_A, 3);
  init_matrix(&expected, values_ex, 3);

  s21_transpose(&A, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(transpose_test6) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);

  double values_A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double values_ex[] = {1, 4, 7, 2, 5, 8, 3, 6, 9};

  init_matrix(&A, values_A, 9);
  init_matrix(&expected, values_ex, 9);

  s21_transpose(&A, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(transpose_fp_test1) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &expected);

  double values_A[] = {1.1, 2.2, 3.3, 4.4};
  double values_ex[] = {1.1, 3.3, 2.2, 4.4};

  init_matrix(&A, values_A, 4);
  init_matrix(&expected, values_ex, 4);

  s21_transpose(&A, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(transpose_fp_test2) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(3, 2, &A);
  s21_create_matrix(2, 3, &expected);

  double values_A[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  double values_ex[] = {1.1, 3.3, 5.5, 2.2, 4.4, 6.6};

  init_matrix(&A, values_A, 6);
  init_matrix(&expected, values_ex, 6);

  s21_transpose(&A, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(transpose_fp_test3) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(1, 3, &A);
  s21_create_matrix(3, 1, &expected);

  double values_A[] = {1.1, 2.2, 3.3};
  double values_ex[] = {1.1, 2.2, 3.3};

  init_matrix(&A, values_A, 3);
  init_matrix(&expected, values_ex, 3);

  s21_transpose(&A, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(transpose_fp_test4) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(3, 1, &A);
  s21_create_matrix(1, 3, &expected);

  double values_A[] = {1.1, 2.2, 3.3};
  double values_ex[] = {1.1, 2.2, 3.3};

  init_matrix(&A, values_A, 3);
  init_matrix(&expected, values_ex, 3);

  s21_transpose(&A, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

Suite *test_s21_transpose(void) {
  Suite *s = suite_create("\033[45m S21_transpose \033[0m");
  TCase *tc_transpose = tcase_create("tc_transpose");

  tcase_add_test(tc_transpose, transpose_test1);
  tcase_add_test(tc_transpose, transpose_test2);
  tcase_add_test(tc_transpose, transpose_test3);
  tcase_add_test(tc_transpose, transpose_test4);
  tcase_add_test(tc_transpose, transpose_test5);
  tcase_add_test(tc_transpose, transpose_test6);

  // Добавленные тесты для значений с плавающей точкой
  tcase_add_test(tc_transpose, transpose_fp_test1);
  tcase_add_test(tc_transpose, transpose_fp_test2);
  tcase_add_test(tc_transpose, transpose_fp_test3);
  tcase_add_test(tc_transpose, transpose_fp_test4);

  suite_add_tcase(s, tc_transpose);
  return s;
}