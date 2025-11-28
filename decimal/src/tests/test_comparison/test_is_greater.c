#include "../decimal_test.h"

START_TEST(is_greater_test1) {
  ///////////////////////////////
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_test2) {
  ///////////////////////////////

  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{0, 1, 0, 0}};
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_test3) {
  ///////////////////////////////

  s21_decimal a = {{1, 0, 1, 0}};
  s21_decimal b = {{1, 1, 0, 0}};
  int result = s21_is_greater(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_test4) {
  ///////////////////////////////

  s21_decimal a = {{0, 0, 1, 0}};
  s21_decimal b = {{0, INT_MAX, 0, 0}};
  int result = s21_is_greater(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_test5) {
  ///////////////////////////////

  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  s21_decimal b = {{INT_MAX, INT_MAX, 1, 0}};
  int result = s21_is_greater(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_test6) {
  ///////////////////////////////

  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&b, 1);
  int result = s21_is_greater(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_test7) {
  ///////////////////////////////

  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&b, 1);
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_test8) {
  ///////////////////////////////

  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_test9) {
  ///////////////////////////////

  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  int result = s21_is_greater(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_test10) {
  ///////////////////////////////

  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 3);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_test11) {
  ///////////////////////////////

  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 2);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_test12) {
  ///////////////////////////////

  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 28);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 29);
  int result = s21_is_greater(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_test13) {
  ///////////////////////////////

  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 14);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 13);
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_neg_test1) {
  ///////////////////////////////
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_neg_test2) {
  ///////////////////////////////
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{0, 1, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_greater(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_neg_test3) {
  ///////////////////////////////
  s21_decimal a = {{1, 0, 1, 0}};
  s21_decimal b = {{1, 1, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_neg_test4) {
  ///////////////////////////////
  s21_decimal a = {{0, 0, 1, 0}};
  s21_decimal b = {{0, INT_MAX, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_neg_test5) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  s21_decimal b = {{INT_MAX, INT_MAX, 1, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_neg_test6) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_neg_test7) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&b, 1);
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_neg_test8) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_neg_test9) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 1);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  set_decimal_sign(&b, 1);
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_neg_test10) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 3);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  set_decimal_sign(&b, 1);
  int result = s21_is_greater(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_neg_test11) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 2);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  set_decimal_sign(&b, 1);
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_neg_test12) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 28);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 29);
  set_decimal_sign(&b, 1);
  int result = s21_is_greater(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_greater_neg_test13) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 14);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 13);
  set_decimal_sign(&b, 1);
  int result = s21_is_greater(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

Suite *test_is_greater(void) {
  Suite *s = suite_create("\033[45m S21_IS_GREATER \033[0m");
  TCase *tc_is_greater = tcase_create("tc_is_greater");

  tcase_add_test(tc_is_greater, is_greater_test1);
  tcase_add_test(tc_is_greater, is_greater_test2);
  tcase_add_test(tc_is_greater, is_greater_test3);
  tcase_add_test(tc_is_greater, is_greater_test4);
  tcase_add_test(tc_is_greater, is_greater_test5);
  tcase_add_test(tc_is_greater, is_greater_test6);
  tcase_add_test(tc_is_greater, is_greater_test7);
  tcase_add_test(tc_is_greater, is_greater_test8);
  tcase_add_test(tc_is_greater, is_greater_test9);
  tcase_add_test(tc_is_greater, is_greater_test10);
  tcase_add_test(tc_is_greater, is_greater_test11);
  tcase_add_test(tc_is_greater, is_greater_test12);
  tcase_add_test(tc_is_greater, is_greater_test13);

  tcase_add_test(tc_is_greater, is_greater_neg_test1);
  tcase_add_test(tc_is_greater, is_greater_neg_test2);
  tcase_add_test(tc_is_greater, is_greater_neg_test3);
  tcase_add_test(tc_is_greater, is_greater_neg_test4);
  tcase_add_test(tc_is_greater, is_greater_neg_test5);
  tcase_add_test(tc_is_greater, is_greater_neg_test6);
  tcase_add_test(tc_is_greater, is_greater_neg_test7);
  tcase_add_test(tc_is_greater, is_greater_neg_test8);
  tcase_add_test(tc_is_greater, is_greater_neg_test9);
  tcase_add_test(tc_is_greater, is_greater_neg_test10);
  tcase_add_test(tc_is_greater, is_greater_neg_test11);
  tcase_add_test(tc_is_greater, is_greater_neg_test12);
  tcase_add_test(tc_is_greater, is_greater_neg_test13);

  suite_add_tcase(s, tc_is_greater);

  return s;
}