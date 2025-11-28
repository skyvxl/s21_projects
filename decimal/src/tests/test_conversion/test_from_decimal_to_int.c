#include "../decimal_test.h"

START_TEST(test_int_max) {
  int expected = INT_MAX, result;
  s21_decimal src = {{INT_MAX, 0, 0, 0}};

  int status = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(result, expected);
  ck_assert_int_eq(status, 0);
}

START_TEST(test_int_min) {
  int expected = INT_MIN, result = 0;
  s21_decimal src = {{INT_MIN, 0, 0, 1 << 31}};

  int status = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(result, expected);
  ck_assert_int_eq(status, 0);
}

START_TEST(test_int_min_pos) {
  s21_decimal src = {{INT_MIN, 0, 0}};
  int result;

  int status = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(status, 1);
}

START_TEST(test_int_pos_random) {
  int sign = rand() % 2;
  int expected = rand() % (sign ? INT_MIN : INT_MAX), result;
  if (sign) expected = -expected;
  s21_decimal src = {{sign ? -expected : expected, 0, 0, sign << 31}};
  int status = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(result, expected);
  ck_assert_int_eq(status, 0);
}

Suite *test_from_decimal_to_int() {
  Suite *s = suite_create("\033[42m\033[30m========= TO INT =========\033[0m");
  TCase *tc = tcase_create("core");

  tcase_add_loop_test(tc, test_int_max, 0, 18);
  tcase_add_loop_test(tc, test_int_min, 0, 18);
  tcase_add_loop_test(tc, test_int_min_pos, 0, 18);
  tcase_add_loop_test(tc, test_int_pos_random, 0, 1000);

  suite_add_tcase(s, tc);
  return s;
}
