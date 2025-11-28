#include "matrix_test.h"

START_TEST(mult_matrix_test1) {
  ///////////////////////////////
  // success creation
  matrix_t A, B, excepted, result;
  double values[] = {0, 1, 2, 3, 4, 5};
  double values_ex[] = {10, 13, 28, 40};

  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);
  s21_create_matrix(2, 2, &excepted);

  init_matrix(&A, values, 6);
  init_matrix(&B, values, 6);
  init_matrix(&excepted, values_ex, 4);

  s21_mult_matrix(&A, &B, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &excepted), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&excepted);
}
END_TEST

START_TEST(mult_matrix_test2) {
  matrix_t A, B, excepted, result;
  double values_A[] = {1, 2, 3, 4};
  double values_B[] = {5, 6, 7, 8};
  double values_ex[] = {19, 22, 43, 50};

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  s21_create_matrix(2, 2, &excepted);

  init_matrix(&A, values_A, 4);
  init_matrix(&B, values_B, 4);
  init_matrix(&excepted, values_ex, 4);

  s21_mult_matrix(&A, &B, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &excepted), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&excepted);
}
END_TEST

START_TEST(mult_matrix_test3) {
  matrix_t A, B, excepted, result;
  double values_A[] = {1, 0, 0, 1};
  double values_B[] = {1, 2, 3, 4};
  double values_ex[] = {1, 2, 3, 4};

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  s21_create_matrix(2, 2, &excepted);

  init_matrix(&A, values_A, 4);
  init_matrix(&B, values_B, 4);
  init_matrix(&excepted, values_ex, 4);

  s21_mult_matrix(&A, &B, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &excepted), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&excepted);
}
END_TEST

START_TEST(mult_matrix_test4) {
  matrix_t A, B, excepted, result;
  double values_A[] = {2, 3, 4, 5};
  double values_B[] = {1, 0, 0, 1};
  double values_ex[] = {2, 3, 4, 5};

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  s21_create_matrix(2, 2, &excepted);

  init_matrix(&A, values_A, 4);
  init_matrix(&B, values_B, 4);
  init_matrix(&excepted, values_ex, 4);

  s21_mult_matrix(&A, &B, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &excepted), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&excepted);
}
END_TEST

START_TEST(mult_matrix_test5) {
  matrix_t A, B, excepted, result;
  double values_A[] = {1, 2, 3, 4, 5, 6};
  double values_B[] = {7, 8, 9, 10, 11, 12};
  double values_ex[] = {58, 64, 139, 154};

  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);
  s21_create_matrix(2, 2, &excepted);

  init_matrix(&A, values_A, 6);
  init_matrix(&B, values_B, 6);
  init_matrix(&excepted, values_ex, 4);

  s21_mult_matrix(&A, &B, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &excepted), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&excepted);
}
END_TEST

START_TEST(mult_matrix_test6) {
  matrix_t A, B, excepted, result;
  double values_A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double values_B[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
  double values_ex[] = {30, 24, 18, 84, 69, 54, 138, 114, 90};

  s21_create_matrix(3, 3, &A);
  s21_create_matrix(3, 3, &B);
  s21_create_matrix(3, 3, &excepted);

  init_matrix(&A, values_A, 9);
  init_matrix(&B, values_B, 9);
  init_matrix(&excepted, values_ex, 9);

  s21_mult_matrix(&A, &B, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &excepted), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&excepted);
}
END_TEST

// * кейсы, вызывающие ошибку

START_TEST(mult_matrix_error_test1) {
  // Ошибка: матрицы не могут быть умножены (разные размеры)
  matrix_t A, B, result;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(4, 2, &B);

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), CALCULATION_ERR);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(mult_matrix_error_test2) {
  // Ошибка: одна из матриц не инициализирована
  matrix_t A, B = {0, 0, 0}, result;
  s21_create_matrix(2, 3, &A);

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), INCORRECT_MATRIX);

  s21_remove_matrix(&A);
}
END_TEST

START_TEST(mult_matrix_error_test3) {
  // Ошибка: одна из матриц имеет нулевые размеры
  matrix_t A, B, result;
  s21_create_matrix(2, 3, &B);
  s21_create_matrix(0, 3, &A);
  A.rows = 0;

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), INCORRECT_MATRIX);

  s21_remove_matrix(&B);
}
END_TEST

START_TEST(mult_matrix_error_test5) {
  // Ошибка: обе матрицы не инициализированы
  matrix_t A = {0, 0, 0}, B = {0, 0, 0}, result;

  ck_assert_int_eq(s21_mult_matrix(&A, &B, &result), INCORRECT_MATRIX);
}
END_TEST

// * кейсы с doubles

START_TEST(mult_matrix_fp_test1) {
  matrix_t A, B, excepted, result;
  double values_A[] = {1.1, 2.2, 3.3, 4.4};
  double values_B[] = {5.5, 6.6, 7.7, 8.8};
  double values_ex[] = {22.99, 26.62, 52.03, 60.5};

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  s21_create_matrix(2, 2, &excepted);

  init_matrix(&A, values_A, 4);
  init_matrix(&B, values_B, 4);
  init_matrix(&excepted, values_ex, 4);

  s21_mult_matrix(&A, &B, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &excepted), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&excepted);
}
END_TEST

START_TEST(mult_matrix_fp_test2) {
  matrix_t A, B, excepted, result;
  double values_A[] = {1.1, 0.0, 0.0, 1.1};
  double values_B[] = {1.1, 2.2, 3.3, 4.4};
  double values_ex[] = {1.21, 2.42, 3.63, 4.84};

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  s21_create_matrix(2, 2, &excepted);

  init_matrix(&A, values_A, 4);
  init_matrix(&B, values_B, 4);
  init_matrix(&excepted, values_ex, 4);

  s21_mult_matrix(&A, &B, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &excepted), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&excepted);
}
END_TEST

START_TEST(mult_matrix_fp_test3) {
  matrix_t A, B, excepted, result;
  double values_A[] = {1.1, 2.2, 3.3, 4.4};
  double values_B[] = {0.1, 0.2, 0.3, 0.4};
  double values_ex[] = {0.77, 1.1, 1.65, 2.42};

  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  s21_create_matrix(2, 2, &excepted);

  init_matrix(&A, values_A, 4);
  init_matrix(&B, values_B, 4);
  init_matrix(&excepted, values_ex, 4);

  s21_mult_matrix(&A, &B, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &excepted), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&excepted);
}
END_TEST

START_TEST(mult_matrix_fp_test4) {
  matrix_t A, B, excepted, result;
  double values_A[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
  double values_B[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6};
  double values_ex[] = {2.42, 3.08, 5.39, 7.04};

  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);
  s21_create_matrix(2, 2, &excepted);

  init_matrix(&A, values_A, 6);
  init_matrix(&B, values_B, 6);
  init_matrix(&excepted, values_ex, 4);

  s21_mult_matrix(&A, &B, &result);

  ck_assert_int_eq(s21_eq_matrix(&result, &excepted), SUCCESS);

  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&result);
  s21_remove_matrix(&excepted);
}
END_TEST

//
//   s21_remove_matrix(&B);
//   s21_remove_matrix(&result);
//   s21_remove_matrix(&excepted);
// }
// END_TEST

Suite *test_s21_mult_matrix(void) {
  Suite *s = suite_create("\033[45m S21_mult_matrix \033[0m");
  TCase *tc_mult_matrix = tcase_create("tc_mult_matrix");

  // * обычные кейсы
  tcase_add_test(tc_mult_matrix, mult_matrix_test1);
  tcase_add_test(tc_mult_matrix, mult_matrix_test2);
  tcase_add_test(tc_mult_matrix, mult_matrix_test3);
  tcase_add_test(tc_mult_matrix, mult_matrix_test4);
  tcase_add_test(tc_mult_matrix, mult_matrix_test5);
  tcase_add_test(tc_mult_matrix, mult_matrix_test6);

  // * кейсы, вызывающие ошибку
  tcase_add_test(tc_mult_matrix, mult_matrix_error_test1);
  tcase_add_test(tc_mult_matrix, mult_matrix_error_test2);
  tcase_add_test(tc_mult_matrix, mult_matrix_error_test3);
  tcase_add_test(tc_mult_matrix, mult_matrix_error_test5);

  // * кейсы для значений с плавающей точкой
  tcase_add_test(tc_mult_matrix, mult_matrix_fp_test1);
  tcase_add_test(tc_mult_matrix, mult_matrix_fp_test2);
  tcase_add_test(tc_mult_matrix, mult_matrix_fp_test3);
  tcase_add_test(tc_mult_matrix, mult_matrix_fp_test4);

  suite_add_tcase(s, tc_mult_matrix);
  return s;
}