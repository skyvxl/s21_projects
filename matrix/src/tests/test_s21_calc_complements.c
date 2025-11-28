#include "matrix_test.h"

START_TEST(calc_complements_testNULL) {
  ///////////////////////////////
  matrix_t A = {0, 0, 0};
  matrix_t *result = NULL;
  ck_assert_int_eq(s21_calc_complements(&A, result), INCORRECT_MATRIX);
}
END_TEST

START_TEST(calc_complements_test1) {
  ///////////////////////////////
  matrix_t A = {0, 0, 0};
  matrix_t result = {0, 0, 0};
  ck_assert_int_eq(s21_calc_complements(&A, &result), INCORRECT_MATRIX);
}
END_TEST

START_TEST(calc_complements_test2) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &expected);

  double values_A[] = {5};
  double values_ex[] = {1};

  init_matrix(&A, values_A, 1);
  init_matrix(&expected, values_ex, 1);

  ck_assert_int_eq(s21_calc_complements(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(calc_complements_test3) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &expected);

  double values_A[] = {1, 2, 3, 4};
  double values_ex[] = {4, -3, -2, 1};

  init_matrix(&A, values_A, 4);
  init_matrix(&expected, values_ex, 4);

  ck_assert_int_eq(s21_calc_complements(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(calc_complements_test_not_square) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0};

  s21_create_matrix(2, 3, &A);

  double values_A[] = {1, 2, 3, 4};

  init_matrix(&A, values_A, 6);

  ck_assert_int_eq(s21_calc_complements(&A, &result), CALCULATION_ERR);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(calc_complements_test4) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);

  double values_A[] = {1, 2, 3, 0, 1, 4, 5, 6, 0};
  double values_ex[] = {-24, 20, -5, 18, -15, 4, 5, -4, 1};

  init_matrix(&A, values_A, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_calc_complements(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(calc_complements_test5) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);

  double values_A[] = {2, -3, 1, 2, 0, -1, 1, 4, 5};
  double values_ex[] = {4, -11, 8, 19, 9, -11, 3, 4, 6};

  init_matrix(&A, values_A, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_calc_complements(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(calc_complements_test6) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(4, 4, &A);
  s21_create_matrix(4, 4, &expected);

  double values_A[] = {1, 0, 2, -1, 3, 0, 0, 5, 2, 1, 4, -3, 1, 0, 5, 0};
  double values_ex[] = {25, -75, -5, -15, 5,   3, -1, 3,
                        0,  30,  0,  0,   -10, 6, 8,  6};
  init_matrix(&A, values_A, 16);
  init_matrix(&expected, values_ex, 16);

  ck_assert_int_eq(s21_calc_complements(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(calc_complements_fp_test1) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &expected);

  double values_A[] = {1.1, 2.2, 3.3, 4.4};
  double values_ex[] = {4.4, -3.3, -2.2, 1.1};

  init_matrix(&A, values_A, 4);
  init_matrix(&expected, values_ex, 4);

  ck_assert_int_eq(s21_calc_complements(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(calc_complements_fp_test2) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);

  double values_A[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9};
  double values_ex[] = {-3.63, 7.26,  -3.63, 7.26, -14.52,
                        7.26,  -3.63, 7.26,  -3.63};

  init_matrix(&A, values_A, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_calc_complements(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(calc_complements_fp_test3) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &expected);

  double values_A[] = {1.1, 0.0, 0.0, 0.0, 1.1, 0.0, 0.0, 0.0, 1.1};
  double values_ex[] = {1.21, 0.0, 0.0, 0.0, 1.21, 0.0, 0.0, 0.0, 1.21};

  init_matrix(&A, values_A, 9);
  init_matrix(&expected, values_ex, 9);

  ck_assert_int_eq(s21_calc_complements(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

START_TEST(calc_complements_fp_test4) {
  ///////////////////////////////
  matrix_t A, result = {0, 0, 0}, expected;

  s21_create_matrix(4, 4, &A);
  s21_create_matrix(4, 4, &expected);

  double values_A[] = {1.1, 0.0, 2.2, -1.1, 3.3, 0.0, 0.0, 5.5,
                       2.2, 1.1, 4.4, -3.3, 1.1, 0.0, 5.5, 0.0};
  double values_ex[] = {33.275, -99.825, -6.655, -19.965, 6.655, 3.993,
                        -1.331, 3.993,   0.0,    39.93,   0.0,   -0.0,
                        -13.31, 7.986,   10.648, 7.986};

  init_matrix(&A, values_A, 16);
  init_matrix(&expected, values_ex, 16);

  ck_assert_int_eq(s21_calc_complements(&A, &result), OK);
  ck_assert_int_eq(s21_eq_matrix(&result, &expected), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&expected);
  s21_remove_matrix(&result);
}
END_TEST

Suite *test_s21_calc_complements(void) {
  Suite *s = suite_create("\033[45m S21_calc_complements \033[0m");
  TCase *tc_calc_complements = tcase_create("tc_calc_complements");

  tcase_add_test(tc_calc_complements, calc_complements_test_not_square);
  tcase_add_test(tc_calc_complements, calc_complements_testNULL);
  tcase_add_test(tc_calc_complements, calc_complements_test1);
  tcase_add_test(tc_calc_complements, calc_complements_test2);
  tcase_add_test(tc_calc_complements, calc_complements_test3);
  tcase_add_test(tc_calc_complements, calc_complements_test4);
  tcase_add_test(tc_calc_complements, calc_complements_test5);
  tcase_add_test(tc_calc_complements, calc_complements_test6);

  // Добавленные тесты для значений с плавающей точкой
  tcase_add_test(tc_calc_complements, calc_complements_fp_test1);
  tcase_add_test(tc_calc_complements, calc_complements_fp_test2);
  tcase_add_test(tc_calc_complements, calc_complements_fp_test3);
  tcase_add_test(tc_calc_complements, calc_complements_fp_test4);

  suite_add_tcase(s, tc_calc_complements);
  return s;
}