#include "matrix_test.h"

START_TEST(inverse_matrix_test1) {
  ///////////////////////////////
  matrix_t A = {0, 0, 0};
  matrix_t result = {0, 0, 0};
  ck_assert_int_eq(s21_inverse_matrix(&A, &result), INCORRECT_MATRIX);
}
END_TEST

START_TEST(inverse_matrix_test2) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &expected);

  double values_A[] = {5};
  double values_ex[] = {0.2};

  init_matrix(&A, values_A, 1);
  init_matrix(&expected, values_ex, 1);

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(inverse_matrix_test3) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &expected);

  double values_A[] = {1, 2, 3, 4};
  double values_ex[] = {-2, 1, 1.5, -0.5};

  init_matrix(&A, values_A, 4);
  init_matrix(&expected, values_ex, 4);

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(inverse_matrix_test4) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);

  double values_A[] = {1, 2, 3, 0, 1, 4, 5, 6, 0};
  double values_ex[] = {-24, 18, 5, 20, -15, -4, -5, 4, 1};

  init_matrix(&A, values_A, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(inverse_matrix_test5) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);

  double values_A[] = {2, -3, 1, 2, 0, -1, 1, 4, 5};
  double values_ex[] = {4. / 49., 19. / 49., 3. / 49.,   -11. / 49., 9. / 49.,
                        4. / 49., 8. / 49.,  -11. / 49., 6. / 49.};

  init_matrix(&A, values_A, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(inverse_matrix_test6) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(4, 4, &A);
  s21_create_matrix(4, 4, &expected);

  double values_A[] = {1, 0, 2, -1, 3, 0, 0, 5, 2, 1, 4, -3, 1, 0, 5, 0};
  double values_ex[] = {
      5.0 / 6.0,  1.0 / 6.0,   0.0, -1.0 / 3.0, -2.5, 0.1, 1.0, 0.2,
      -1.0 / 6.0, -1.0 / 30.0, 0.0, 4.0 / 15.0, -0.5, 0.1, 0.0, 0.2};

  init_matrix(&A, values_A, 16);
  init_matrix(&expected, values_ex, 16);

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(inverse_matrix_fp_test1) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &expected);

  double values_A[] = {1.1, 2.2, 3.3, 4.4};
  double values_ex[] = {-20. / 11., 10. / 11., 15. / 11., -5. / 11.};

  init_matrix(&A, values_A, 4);
  init_matrix(&expected, values_ex, 4);

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(inverse_matrix_fp_test2) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);

  double values_A[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9};
  double values_ex[] = {
      5.0 / 6.0,  1.0 / 6.0,   0.0, -1.0 / 3.0, -2.5, 0.1, 1.0, 0.2,
      -1.0 / 6.0, -1.0 / 30.0, 0.0, 4.0 / 15.0, -0.5, 0.1, 0.0, 0.2};

  init_matrix(&A, values_A, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), CALCULATION_ERR);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(inverse_matrix_fp_test3) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);

  double values_A[] = {1.1, 0.0, 0.0, 0.0, 1.1, 0.0, 0.0, 0.0, 1.1};
  double values_ex[] = {10. / 11., 0.0, 0.0, 0.0,      10. / 11.,
                        0.0,       0.0, 0.0, 10. / 11.};

  init_matrix(&A, values_A, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(inverse_matrix_fp_test4) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(4, 4, &A);
  s21_create_matrix(4, 4, &expected);

  double values_A[] = {1.1, 0.0, 2.2, -1.1, 3.3, 0.0, 0.0, 5.5,
                       2.2, 1.1, 4.4, -3.3, 1.1, 0.0, 5.5, 0.0};
  double values_ex[] = {25. / 33.,  5. / 33.,  0,         -10. / 33.,
                        -25. / 11., 1. / 11.,  10. / 11., 2. / 11.,
                        -5. / 33.,  -1. / 33., 0,         8. / 33.,
                        -5. / 11.,  1. / 11.,  0,         2. / 11.};

  init_matrix(&A, values_A, 16);
  init_matrix(&expected, values_ex, 16);

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(inverse_matrix_test_not_square) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0};

  s21_create_matrix(2, 3, &A);

  double values_A[] = {1, 2, 3, 4};

  init_matrix(&A, values_A, 6);

  ck_assert_int_eq(s21_inverse_matrix(&A, &result), CALCULATION_ERR);

  s21_remove_matrix(&A);
}
END_TEST

Suite *test_s21_inverse_matrix(void) {
  Suite *s = suite_create("\033[45m S21_inverse_matrix \033[0m");
  TCase *tc_inverse_matrix = tcase_create("tc_inverse_matrix");

  tcase_add_test(tc_inverse_matrix, inverse_matrix_test1);
  tcase_add_test(tc_inverse_matrix, inverse_matrix_test2);
  tcase_add_test(tc_inverse_matrix, inverse_matrix_test3);
  tcase_add_test(tc_inverse_matrix, inverse_matrix_test4);
  tcase_add_test(tc_inverse_matrix, inverse_matrix_test5);
  tcase_add_test(tc_inverse_matrix, inverse_matrix_test6);

  // Добавленные тесты для значений с плавающей точкой
  tcase_add_test(tc_inverse_matrix, inverse_matrix_fp_test1);
  tcase_add_test(tc_inverse_matrix, inverse_matrix_fp_test2);
  tcase_add_test(tc_inverse_matrix, inverse_matrix_fp_test3);
  tcase_add_test(tc_inverse_matrix, inverse_matrix_fp_test4);

  tcase_add_test(tc_inverse_matrix, inverse_matrix_test_not_square);

  suite_add_tcase(s, tc_inverse_matrix);
  return s;
}