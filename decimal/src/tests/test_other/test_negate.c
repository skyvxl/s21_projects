#include "../decimal_test.h"

START_TEST(negate_test1) {
  ///////////////////////////////
  s21_decimal temp = {{725, 0, 0, 512}};
  set_decimal_sign(&temp, 0);
  s21_decimal res;
  s21_negate(temp, &res);
  s21_decimal expected = {{725, 0, 0, 512}};
  set_decimal_sign(&expected, 1);
  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(negate_test2) {
  ///////////////////////////////
  s21_decimal temp = {{1, 0, 0, 0}};
  s21_decimal res;
  s21_negate(temp, &res);
  s21_decimal expected = {{1, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(negate_test3) {
  ///////////////////////////////
  s21_decimal temp = {{1, 0, 0, 0}};
  set_decimal_sign(&temp, 1);
  s21_decimal res;
  s21_negate(temp, &res);
  s21_decimal expected = {{1, 0, 0, 0}};
  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(negate_test4) {
  ///////////////////////////////
  s21_decimal temp = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&temp, 1);
  s21_decimal res;
  s21_negate(temp, &res);
  s21_decimal expected = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(negate_test5) {
  ///////////////////////////////
  s21_decimal temp = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  s21_decimal res;
  s21_negate(temp, &res);
  s21_decimal expected = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&expected, 1);
  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(negate_test6) {
  ///////////////////////////////
  s21_decimal temp = {{INT_MAX, INT_MAX, INT_MAX, INT_MAX}};
  s21_decimal res;
  s21_negate(temp, &res);
  s21_decimal expected = {{INT_MAX, INT_MAX, INT_MAX, INT_MAX}};
  set_decimal_sign(&expected, 1);
  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

Suite *test_negate(void) {
  Suite *s = suite_create("\033[45m S21_NEGATE \033[0m");
  TCase *tc_negate = tcase_create("tc_negate");

  tcase_add_test(tc_negate, negate_test1);
  tcase_add_test(tc_negate, negate_test2);
  tcase_add_test(tc_negate, negate_test3);
  tcase_add_test(tc_negate, negate_test4);
  tcase_add_test(tc_negate, negate_test5);
  tcase_add_test(tc_negate, negate_test6);

  suite_add_tcase(s, tc_negate);
  return s;
}