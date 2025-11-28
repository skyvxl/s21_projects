#include "../decimal_test.h"

START_TEST(test_int_min) {
  int src = INT_MIN;
  s21_decimal expected = {{INT_MAX + 1U, 0, 0, 1 << 31}};
  s21_decimal result = {{0}};
  int status = s21_from_int_to_decimal(src, &result);

  ck_assert_msg(s21_is_equal(result, expected), "test_int_min failed");
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_int_max) {
  int src = INT_MAX;
  s21_decimal expected = {{INT_MAX, 0, 0, 0}};
  s21_decimal result = {{0}};
  int status = s21_from_int_to_decimal(src, &result);

  ck_assert_msg(s21_is_equal(result, expected), "test_int_max failed");
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_int_zero) {
  int src = 0;
  s21_decimal expected = {{0}};
  s21_decimal result = {{0}};
  int status = s21_from_int_to_decimal(src, &result);

  ck_assert_msg(s21_is_equal(result, expected), "test_int_zero failed");
  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(test_int_random) {
  int src = rand() % INT_MAX;
  int sign = rand() % 2;
  s21_decimal expected = {{src, 0, 0, sign << 31}};
  s21_decimal result = {{0}};
  if (sign) src = -src;
  int status = s21_from_int_to_decimal(src, &result);

  ck_assert_msg(s21_is_equal(result, expected), "test_int_random failed");
  ck_assert_int_eq(status, 0);
}
END_TEST

Suite *test_from_int_to_decimal() {
  Suite *s = suite_create(
      "\033[42m\033[30m========= FROM INT TO DECIMAL =========\033[0m");
  TCase *tc = tcase_create("core");

  tcase_add_test(tc, test_int_min);
  tcase_add_test(tc, test_int_max);
  tcase_add_test(tc, test_int_zero);
  tcase_add_loop_test(tc, test_int_random, 0, 1000);
  suite_add_tcase(s, tc);
  return s;
}
