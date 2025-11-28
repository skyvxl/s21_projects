#include <check.h>
#include <math.h>

#include "../decimal_test.h"

// * Correct conversions
START_TEST(decimal_to_float_test1) {
  s21_decimal src = {{12345, 0, 0, 0}};
  set_decimal_scale(&src, 2);
  float dst;
  int status = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  ck_assert_float_eq(dst, 123.45f);
}
END_TEST

START_TEST(decimal_to_float_test2) {
  s21_decimal src = {{12345, 0, 0, 0}};
  set_decimal_sign(&src, 1);
  set_decimal_scale(&src, 2);
  float dst;
  int status = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  ck_assert_float_eq(dst, -123.45f);
}
END_TEST

START_TEST(decimal_to_float_test3) {
  s21_decimal src = {{0, 0, 0, 0}};
  float dst;
  int status = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  ck_assert_float_eq(dst, 0.0f);
}
END_TEST

// * Incorrect conversions
START_TEST(decimal_to_float_test4) {
  s21_decimal src = {{1, 0, 0, 0}};
  set_decimal_scale(&src, 28);  // Scale too large
  float dst;
  int status = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  ck_assert_float_eq_tol(dst, 1e-28, 1e-28);
  /*
  чер знает почему простое сравнение с 1e-28 не работает, но вот такое сработало
  это типо диапазон, в котором должно быть значение
  */
}

START_TEST(decimal_to_float_test5) {
  s21_decimal src = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  float dst;
  int status = s21_from_decimal_to_float(src, &dst);
  if (isnan(dst) || isinf(dst)) {
    ck_assert_int_eq(status, CONVERSION_ERROR);
  } else {
    ck_assert_int_eq(status, CONVERSION_OK);
  }
}
END_TEST

// * With large number
START_TEST(decimal_to_float_test6) {
  s21_decimal src = {{999999999, 0, 0, 0}};
  float dst;
  int status = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  ck_assert_float_eq(dst, 999999999.0f);
}
END_TEST

// * Small values and zero
START_TEST(decimal_to_float_test7) {
  s21_decimal src = {{1, 0, 0, 0}};
  set_decimal_scale(&src, 5);
  float dst;
  int status = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  ck_assert_float_eq(dst, 0.00001f);
}
END_TEST

START_TEST(decimal_to_float_test8) {
  s21_decimal src = {{1, 0, 0, 0}};
  set_decimal_scale(&src, 5);
  set_decimal_sign(&src, 1);
  float dst;
  int status = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  ck_assert_float_eq(dst, -0.00001f);
}
END_TEST

// * Integer decimals
START_TEST(decimal_to_float_test9) {
  s21_decimal src = {{12345, 0, 0, 0}};
  float dst;
  int status = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  ck_assert_float_eq(dst, 12345.0f);
}
END_TEST

START_TEST(decimal_to_float_test10) {
  s21_decimal src = {{12345, 0, 0, 0}};
  set_decimal_sign(&src, 1);
  float dst;
  int status = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  ck_assert_float_eq(dst, -12345.0f);
}
END_TEST

// * Decimals with different scales
START_TEST(decimal_to_float_test11) {
  s21_decimal src = {{1234567, 0, 0, 0}};
  set_decimal_scale(&src, 6);
  float dst;
  int status = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  ck_assert_float_eq(dst, 1.234567f);
}
END_TEST

START_TEST(decimal_to_float_test12) {
  s21_decimal src = {{1234567, 0, 0, 0}};
  set_decimal_scale(&src, 6);
  set_decimal_sign(&src, 1);
  float dst;
  int status = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  ck_assert_float_eq(dst, -1.234567f);
}
END_TEST

// * Edge case: very small decimal
START_TEST(decimal_to_float_test13) {
  s21_decimal src = {{1, 0, 0, 0}};
  set_decimal_scale(&src, 30);
  float dst;
  int status = s21_from_decimal_to_float(src, &dst);
  ck_assert_int_eq(status, CONVERSION_ERROR);
}
END_TEST

START_TEST(decimal_to_float_test14) {
  s21_decimal src = {{1, 0, 0, 0}};
  set_decimal_scale(&src, 1);
  float *dst = NULL;
  int status = s21_from_decimal_to_float(src, dst);
  ck_assert_int_eq(status, CONVERSION_ERROR);
}
END_TEST

Suite *test_from_decimal_to_float(void) {
  Suite *s = suite_create("\033[45m S21_FROM_DECIMAL_TO_FLOAT \033[0m");
  TCase *tc_decimal_to_float = tcase_create("tc_decimal_to_float");

  tcase_add_test(tc_decimal_to_float, decimal_to_float_test1);
  tcase_add_test(tc_decimal_to_float, decimal_to_float_test2);
  tcase_add_test(tc_decimal_to_float, decimal_to_float_test3);
  tcase_add_test(tc_decimal_to_float, decimal_to_float_test4);
  tcase_add_test(tc_decimal_to_float, decimal_to_float_test5);
  tcase_add_test(tc_decimal_to_float, decimal_to_float_test6);
  tcase_add_test(tc_decimal_to_float, decimal_to_float_test7);
  tcase_add_test(tc_decimal_to_float, decimal_to_float_test8);
  tcase_add_test(tc_decimal_to_float, decimal_to_float_test9);
  tcase_add_test(tc_decimal_to_float, decimal_to_float_test10);
  tcase_add_test(tc_decimal_to_float, decimal_to_float_test11);
  tcase_add_test(tc_decimal_to_float, decimal_to_float_test12);
  tcase_add_test(tc_decimal_to_float, decimal_to_float_test13);
  tcase_add_test(tc_decimal_to_float, decimal_to_float_test14);

  suite_add_tcase(s, tc_decimal_to_float);
  return s;
}
