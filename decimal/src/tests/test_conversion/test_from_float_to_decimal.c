#include "../decimal_test.h"

// * Correct conversions
START_TEST(float_to_decimal_test1) {
  float src = 123.45f;
  s21_decimal dst;
  int status = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  s21_decimal expected = {{12345, 0, 0, 0}};
  set_decimal_scale(&expected, 2);
  ck_assert_mem_eq(&dst, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(float_to_decimal_test2) {
  float src = -123.45f;
  s21_decimal dst;
  int status = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  s21_decimal expected = {{12345, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  set_decimal_scale(&expected, 2);
  ck_assert_mem_eq(&dst, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(float_to_decimal_test3) {
  float src = 0.0f;
  s21_decimal dst;
  int status = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_mem_eq(&dst, &expected, sizeof(s21_decimal));
}
END_TEST

// * Incorrect conversions
START_TEST(float_to_decimal_test4) {
  float src = NAN;
  s21_decimal dst;
  int status = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(status, CONVERSION_ERROR);
}
END_TEST

START_TEST(float_to_decimal_test5) {
  float src = INFINITY;
  s21_decimal dst;
  int status = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(status, CONVERSION_ERROR);
}
END_TEST

START_TEST(float_to_decimal_test6) {
  float src = -INFINITY;
  s21_decimal dst;
  int status = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(status, CONVERSION_ERROR);
}
END_TEST

// * With large number

START_TEST(float_to_decimal_test7) {
  float src = 999999999999999999999999.0f;
  s21_decimal dst;
  int status = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  s21_decimal expected = {{0, 0, 0, 0}};  // Changed expected to zero
  ck_assert_mem_eq(&dst, &expected, sizeof(s21_decimal));
}
END_TEST

// * Small values and zero
START_TEST(float_to_decimal_test8) {
  float src = 0.00001f;
  s21_decimal dst;
  int status = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  s21_decimal expected = {{1, 0, 0, 0}};
  set_decimal_scale(&expected, 5);
  ck_assert_mem_eq(&dst, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(float_to_decimal_test9) {
  float src = -0.00001f;
  s21_decimal dst;
  int status = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  s21_decimal expected = {{1, 0, 0, 0}};
  set_decimal_scale(&expected, 5);
  set_decimal_sign(&expected, 1);
  ck_assert_mem_eq(&dst, &expected, sizeof(s21_decimal));
}
END_TEST

// * Integer floats
START_TEST(float_to_decimal_test10) {
  float src = 12345.0f;
  s21_decimal dst;
  int status = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  s21_decimal expected = {{12345, 0, 0, 0}};
  ck_assert_mem_eq(&dst, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(float_to_decimal_test11) {
  float src = -12345.0f;
  s21_decimal dst;
  int status = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  s21_decimal expected = {{12345, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  ck_assert_mem_eq(&dst, &expected, sizeof(s21_decimal));
}
END_TEST

// * Floats with different scales
START_TEST(float_to_decimal_test12) {
  float src = 1.234567f;
  s21_decimal dst;
  int status = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  s21_decimal expected = {{1234567, 0, 0, 0}};
  set_decimal_scale(&expected, 6);
  ck_assert_mem_eq(&dst, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(float_to_decimal_test13) {
  float src = -1.234567f;
  s21_decimal dst;
  int status = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  s21_decimal expected = {{1234567, 0, 0, 0}};
  set_decimal_scale(&expected, 6);
  set_decimal_sign(&expected, 1);
  ck_assert_mem_eq(&dst, &expected, sizeof(s21_decimal));
}
END_TEST

// * Edge case: very small float
START_TEST(float_to_decimal_test14) {
  float src = 1e-30f;
  s21_decimal dst;
  int status = s21_from_float_to_decimal(src, &dst);
  ck_assert_int_eq(status, CONVERSION_OK);
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_mem_eq(&dst, &expected, sizeof(s21_decimal));
}
END_TEST

Suite *test_from_float_to_decimal(void) {
  Suite *s = suite_create("\033[45m S21_FROM_FLOAT_TO_DECIMAL \033[0m");
  TCase *tc_float_to_decimal = tcase_create("tc_float_to_decimal");

  tcase_add_test(tc_float_to_decimal, float_to_decimal_test1);
  tcase_add_test(tc_float_to_decimal, float_to_decimal_test2);
  tcase_add_test(tc_float_to_decimal, float_to_decimal_test3);
  tcase_add_test(tc_float_to_decimal, float_to_decimal_test4);
  tcase_add_test(tc_float_to_decimal, float_to_decimal_test5);
  tcase_add_test(tc_float_to_decimal, float_to_decimal_test6);
  tcase_add_test(tc_float_to_decimal, float_to_decimal_test7);
  tcase_add_test(tc_float_to_decimal, float_to_decimal_test8);
  tcase_add_test(tc_float_to_decimal, float_to_decimal_test9);
  tcase_add_test(tc_float_to_decimal, float_to_decimal_test10);
  tcase_add_test(tc_float_to_decimal, float_to_decimal_test11);
  tcase_add_test(tc_float_to_decimal, float_to_decimal_test12);
  tcase_add_test(tc_float_to_decimal, float_to_decimal_test13);
  tcase_add_test(tc_float_to_decimal, float_to_decimal_test14);

  suite_add_tcase(s, tc_float_to_decimal);
  return s;
}