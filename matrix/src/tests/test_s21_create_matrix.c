#include "matrix_test.h"

START_TEST(create_matrix_test1) {
  ///////////////////////////////
  // success creation
  matrix_t A = {0, 0, 0};
  ck_assert_int_eq(s21_create_matrix(5, 5, &A), OK);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(create_matrix_test2) {
  // failure null pointer
  ck_assert_int_eq(s21_create_matrix(5, 5, NULL), INCORRECT_MATRIX);
}
END_TEST

START_TEST(create_matrix_test3) {
  matrix_t A = {0, 0, 0};
  ck_assert_int_eq(s21_create_matrix(0, 1, &A), INCORRECT_MATRIX);
}
END_TEST

START_TEST(create_matrix_test4) {
  matrix_t A = {0, 0, 0};
  ck_assert_int_eq(s21_create_matrix(1, 0, &A), INCORRECT_MATRIX);
}
END_TEST

START_TEST(create_matrix_test5) {
  matrix_t A = {0, 0, 0};
  ck_assert_int_eq(s21_create_matrix(1, 1, &A), OK);
  s21_remove_matrix(&A);
}
END_TEST

// !ОЧЕНЬ ЖЕСТКИЕ ТЕСТЫ!
// START_TEST(create_matrix_test6) {
//   matrix_t A = {};
//   ck_assert_int_eq(s21_create_matrix(INT_MAX, 1, &A), INCORRECT_MATRIX);
//   s21_remove_matrix(&A);
// }
// END_TEST

// START_TEST(create_matrix_test7) {
//   matrix_t A = {};
//   ck_assert_int_eq(s21_create_matrix(1, INT_MAX, &A), INCORRECT_MATRIX);
//   s21_remove_matrix(&A);
// }
// END_TEST

// START_TEST(create_matrix_test8) {
//   matrix_t A = {};
//   ck_assert_int_eq(s21_create_matrix(INT_MAX, INT_MAX, &A),
//   INCORRECT_MATRIX); s21_remove_matrix(&A);
// }
// END_TEST

Suite *test_s21_create_matrix(void) {
  Suite *s = suite_create("\033[45m S21_CREATE_MATRIX \033[0m");
  TCase *tc_create_matrix = tcase_create("tc_create_matrix");

  tcase_add_test(tc_create_matrix, create_matrix_test1);
  tcase_add_test(tc_create_matrix, create_matrix_test2);
  tcase_add_test(tc_create_matrix, create_matrix_test3);
  tcase_add_test(tc_create_matrix, create_matrix_test4);
  tcase_add_test(tc_create_matrix, create_matrix_test5);
  // !ОЧЕНЬ ЖЕСТКИЕ ТЕСТЫ!
  // 16Gb
  // tcase_add_test(tc_create_matrix, create_matrix_test6);
  // 16Gb
  // tcase_add_test(tc_create_matrix, create_matrix_test7);
  // 256Gb =)
  // tcase_add_test(tc_create_matrix, create_matrix_test8);

  suite_add_tcase(s, tc_create_matrix);
  return s;
}