#include "matrix_test.h"

START_TEST(sub_matrix_test0) {
  matrix_t A = {0, 0, 0};
  matrix_t B = {0, 0, 0};
  matrix_t result = {0, 0, 0};
  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), INCORRECT_MATRIX);
}
END_TEST

START_TEST(sub_matrix_test1) {
  matrix_t A, B, result;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);

  double values_A[] = {1, 2, 3, 4};
  double values_B[] = {5, 6, 7, 8};
  init_matrix(&A, values_A, 4);
  init_matrix(&B, values_B, 4);

  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), OK);
  ck_assert_double_eq(result.matrix[0][0], -4);
  ck_assert_double_eq(result.matrix[0][1], -4);
  ck_assert_double_eq(result.matrix[1][0], -4);
  ck_assert_double_eq(result.matrix[1][1], -4);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(sub_matrix_test2) {
  matrix_t A, B, result;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);

  double values_A[] = {0, 1, 2, 1, 2, 3, 2, 3, 4};
  double values_B[] = {0, 1, 2, 1, 2, 3, 2, 3, 4};
  init_matrix(&A, values_A, 9);
  init_matrix(&B, values_B, 9);

  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), OK);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq(result.matrix[i][j], 0);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(sub_matrix_test3) {
  matrix_t A, B, result;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(2, 3, &B);
  result.matrix = NULL;

  double values_A[] = {1, 2, 3, 4, 5, 6};
  double values_B[] = {1, 2, 3, 4, 5, 6};
  init_matrix(&A, values_A, 6);
  init_matrix(&B, values_B, 6);

  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), OK);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_double_eq(result.matrix[i][j], 0.);
    }
  }

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(sub_matrix_test4) {
  matrix_t A, B;
  matrix_t *result = NULL;
  s21_create_matrix(3, 3, &A);
  s21_create_matrix(2, 3, &B);

  double values_A[] = {1, 2, 3, 4, 5, 6};
  double values_B[] = {6, 5, 4, 3, 2, 1};
  init_matrix(&A, values_A, 6);
  init_matrix(&B, values_B, 6);

  ck_assert_int_eq(s21_sub_matrix(&A, &B, result), INCORRECT_MATRIX);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sub_matrix_test5) {
  matrix_t A, B, result = {0, 0, 0};
  A = result;

  s21_create_matrix(1, 0, &A);
  s21_create_matrix(1, 1, &B);

  B.matrix[0][0] = 21;

  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), INCORRECT_MATRIX);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(sub_matrix_test6) {
  ///////////////////////////////
  matrix_t A, B, result = {0, 0, 0};
  A = result;

  s21_create_matrix(1, 2, &A);
  s21_create_matrix(1, 1, &B);

  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), CALCULATION_ERR);
  s21_remove_matrix(&B);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(sub_matrix_test7) {
  ///////////////////////////////
  matrix_t A, B, result = {0, 0, 0};
  A = result;

  s21_create_matrix(1, 1, &A);
  s21_create_matrix(2, 1, &B);

  ck_assert_int_eq(s21_sub_matrix(&A, &B, &result), CALCULATION_ERR);
  s21_remove_matrix(&B);
  s21_remove_matrix(&A);
}
END_TEST

Suite *test_s21_sub_matrix(void) {
  Suite *s = suite_create("\033[45m S21_sub_matrix \033[0m");
  TCase *tc_sub_matrix = tcase_create("tc_sub_matrix");

  tcase_add_test(tc_sub_matrix, sub_matrix_test0);
  tcase_add_test(tc_sub_matrix, sub_matrix_test1);
  tcase_add_test(tc_sub_matrix, sub_matrix_test2);
  tcase_add_test(tc_sub_matrix, sub_matrix_test3);
  tcase_add_test(tc_sub_matrix, sub_matrix_test4);
  tcase_add_test(tc_sub_matrix, sub_matrix_test5);
  tcase_add_test(tc_sub_matrix, sub_matrix_test6);
  tcase_add_test(tc_sub_matrix, sub_matrix_test7);

  suite_add_tcase(s, tc_sub_matrix);
  return s;
}