#include "../decimal_test.h"

START_TEST(is_equal_test_null) {
  ///////////////////////////////
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test1) {
  ///////////////////////////////
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test2) {
  ///////////////////////////////
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test3) {
  ///////////////////////////////
  s21_decimal a = {{1, 0, 1, 0}};
  s21_decimal b = {{1, 0, 1, 0}};
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test4) {
  ///////////////////////////////
  s21_decimal a = {{0, 0, 1, 0}};
  s21_decimal b = {{0, 0, 1, 0}};
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test5) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test6) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test7) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test8) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test9) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test10) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 3);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test11) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 2);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test12) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 28);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 29);
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test13) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 14);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 13);
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test14) {
  ///////////////////////////////
  s21_decimal a = {{0, 0, 0, 0}};
  set_decimal_scale(&a, 14);
  s21_decimal b = {{0, 0, 0, 0}};
  set_decimal_scale(&b, 13);
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test15) {
  ///////////////////////////////
  s21_decimal a = {{1000, 0, 0, 0}};
  set_decimal_scale(&a, 3);
  s21_decimal b = {{10, 0, 0, 0}};
  set_decimal_scale(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test16) {
  ///////////////////////////////
  s21_decimal a = {{1000, 0, 0, 0}};
  set_decimal_scale(&a, 13);
  s21_decimal b = {{10, 0, 0, 0}};
  set_decimal_scale(&b, 13);
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

// ! negative tests

START_TEST(is_equal_neg_test1) {
  ///////////////////////////////
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_neg_test2) {
  ///////////////////////////////
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{0, 1, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_neg_test3) {
  ///////////////////////////////
  s21_decimal a = {{1, 0, 1, 0}};
  s21_decimal b = {{1, 1, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_neg_test4) {
  ///////////////////////////////
  s21_decimal a = {{0, 0, 1, 0}};
  s21_decimal b = {{0, INT_MAX, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_neg_test5) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  s21_decimal b = {{INT_MAX, INT_MAX, 1, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_neg_test6) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_neg_test7) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_neg_test8) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_neg_test9) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 1);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_neg_test10) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 3);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_neg_test11) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 2);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_neg_test12) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 28);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 29);
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_neg_test13) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 14);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 13);
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

// ? +0 == -0 ?

START_TEST(is_equal_null_test) {
  ///////////////////////////////
  s21_decimal a = {{0, 0, 0, 0}};
  set_decimal_scale(&a, 14);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{0, 0, 0, 0}};
  set_decimal_scale(&b, 13);
  set_decimal_sign(&b, 1);
  int result = s21_is_equal(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_equal_test_15) {
  ///////////////////////////////
  s21_decimal a = {{UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX}};
  set_decimal_scale(&a, 28);
  set_decimal_sign(&a, 0);
  s21_decimal b = {{UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX}};
  set_decimal_scale(&b, 27);
  set_decimal_sign(&b, 0);
  int result = s21_is_equal(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(test_1) {
  s21_decimal value_1, value_2;

  value_1.bits[0] = 0b11111111111111111111111111111111;

  value_1.bits[1] = 0b11111111111111111111111111111111;

  value_1.bits[2] = 0b01111111111111111111111111111111;

  value_1.bits[3] = 0b10000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000001;

  value_2.bits[1] = 0b00000000000000000000000000000000;

  value_2.bits[2] = 0b00000000000000000000000000000000;

  value_2.bits[3] = 0b00000000000000000000000000000000;

  ck_assert_int_eq(s21_is_equal(value_1, value_2), 0);
}

END_TEST

START_TEST(test_2) {
  s21_decimal value_1, value_2;

  value_1.bits[0] = 0b00000000000000000000000000000001;

  value_1.bits[1] = 0b00000000000000000000000000000000;

  value_1.bits[2] = 0b00000000000000000000000000000000;

  value_1.bits[3] = 0b10000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000001;

  value_2.bits[1] = 0b00000000000000000000000000000000;

  value_2.bits[2] = 0b00000000000000000000000000000000;

  value_2.bits[3] = 0b00000000000000000000000000000000;

  ck_assert_int_eq(s21_is_equal(value_1, value_2), 0);
}

END_TEST

START_TEST(test_3) {
  s21_decimal value_1, value_2;

  value_1.bits[0] = 0b00000000000000000000000000000001;

  value_1.bits[1] = 0b00000000000000000000000000000000;

  value_1.bits[2] = 0b00000000000000000000000000000000;

  value_1.bits[3] = 0b10000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000001;

  value_2.bits[1] = 0b00000000000000000000000000000000;

  value_2.bits[2] = 0b00000000000000000000000000000000;

  value_2.bits[3] = 0b10000000000000000000000000000000;

  ck_assert_int_eq(s21_is_equal(value_1, value_2), 1);
}

END_TEST

START_TEST(test_4) {
  s21_decimal value_1, value_2;

  value_1.bits[0] = 0b00000000000000000000000000000010;

  value_1.bits[1] = 0b00000000000000000000000000000000;

  value_1.bits[2] = 0b00000000000000000000000000000000;

  value_1.bits[3] = 0b00000000000000100000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000010;

  value_2.bits[1] = 0b00000000000000000000000000000000;

  value_2.bits[2] = 0b00000000000000000000000000000000;

  value_2.bits[3] = 0b00000000000000000000000000000000;

  ck_assert_int_eq(s21_is_equal(value_1, value_2), 0);
}

END_TEST

START_TEST(test_5) {
  s21_decimal value_1, value_2;

  value_1.bits[0] = 0b00000000000000000000000000000010;

  value_1.bits[1] = 0b00000000000000000000000000000000;

  value_1.bits[2] = 0b00000000000000000000000000000000;

  value_1.bits[3] = 0b10000000000000100000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000010;

  value_2.bits[1] = 0b00000000000000000000000000000000;

  value_2.bits[2] = 0b00000000000000000000000000000000;

  value_2.bits[3] = 0b10000000000000000000000000000000;

  ck_assert_int_eq(s21_is_equal(value_1, value_2), 0);
}

END_TEST

START_TEST(test_6) {
  s21_decimal value_1, value_2;

  value_1.bits[0] = 0b00000000000000000000000000000000;

  value_1.bits[1] = 0b00000000000000000000000000000000;

  value_1.bits[2] = 0b00100000000000000000000000000000;

  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000000;

  value_2.bits[1] = 0b00000000000000000000000000000000;

  value_2.bits[2] = 0b01000000000000000000000000000000;

  value_2.bits[3] = 0b00000000000000000000000000000000;

  ck_assert_int_eq(s21_is_equal(value_1, value_2), 0);
}

END_TEST

START_TEST(test_7) {
  s21_decimal value_1, value_2;

  value_1.bits[0] = 0b00000000000000000000000000000010;

  value_1.bits[1] = 0b00000000000000000000000000000001;

  value_1.bits[2] = 0b00000000000000000000000000000000;

  value_1.bits[3] = 0b00000000000110010000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000010;

  value_2.bits[1] = 0b00000000000000000000000000000001;

  value_2.bits[2] = 0b00000000000000000000000000000000;

  value_2.bits[3] = 0b00000000000000000000000000000000;

  ck_assert_int_eq(s21_is_equal(value_1, value_2), 0);
}

END_TEST

START_TEST(test_8) {
  s21_decimal value_1, value_2;

  value_1.bits[0] = 0b11111111111111111111111111111111;

  value_1.bits[1] = 0b11111111111111111111111111111111;

  value_1.bits[2] = 0b01111111111111111111111111111111;

  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b11111111111111111111111111111111;

  value_2.bits[1] = 0b11111111111111111111111111111111;

  value_2.bits[2] = 0b01111111111111111111111111111111;

  value_2.bits[3] = 0b00000000000000000000000000000000;

  ck_assert_int_eq(s21_is_equal(value_1, value_2), 1);
}

END_TEST

START_TEST(test_9) {
  s21_decimal value_1, value_2;

  value_1.bits[0] = 0b11111111111111111111111111111111;

  value_1.bits[1] = 0b11111111111111111111111111111111;

  value_1.bits[2] = 0b01111111111111111111111111111111;

  value_1.bits[3] = 0b10000000000000000000000000000000;

  value_2.bits[0] = 0b11111111111111111111111111111111;

  value_2.bits[1] = 0b11111111111111111111111111111111;

  value_2.bits[2] = 0b01111111111111111111111111111111;

  value_2.bits[3] = 0b10000000000000000000000000000000;

  ck_assert_int_eq(s21_is_equal(value_1, value_2), 1);
}

END_TEST

// ? hard tests

START_TEST(is_equal_hard1) {
  s21_decimal value_1, value_2;
  clean_decimal(&value_1);
  clean_decimal(&value_2);

  value_1.bits[0] = 0b01110001111110110000100001000011;

  value_1.bits[1] = 0b00000000000000000000000100011111;

  set_decimal_scale(&value_1, 12);

  value_2.bits[0] = 0b01110011110011100101001010011110;

  value_2.bits[1] = 0b00000000000000000000101100111010;

  set_decimal_scale(&value_2, 13);

  ck_assert_int_eq(s21_is_equal(value_1, value_2), 1);
}

END_TEST

Suite *test_is_equal(void) {
  Suite *s = suite_create("\033[45m S21_IS_EQUAL \033[0m");
  TCase *tc_is_equal = tcase_create("tc_is_equal");

  tcase_add_test(tc_is_equal, is_equal_test_null);
  tcase_add_test(tc_is_equal, is_equal_test1);
  tcase_add_test(tc_is_equal, is_equal_test2);
  tcase_add_test(tc_is_equal, is_equal_test3);
  tcase_add_test(tc_is_equal, is_equal_test4);
  tcase_add_test(tc_is_equal, is_equal_test5);
  tcase_add_test(tc_is_equal, is_equal_test6);
  tcase_add_test(tc_is_equal, is_equal_test7);
  tcase_add_test(tc_is_equal, is_equal_test8);
  tcase_add_test(tc_is_equal, is_equal_test9);
  tcase_add_test(tc_is_equal, is_equal_test10);
  tcase_add_test(tc_is_equal, is_equal_test11);
  tcase_add_test(tc_is_equal, is_equal_test12);
  tcase_add_test(tc_is_equal, is_equal_test13);
  tcase_add_test(tc_is_equal, is_equal_test14);
  tcase_add_test(tc_is_equal, is_equal_test15);
  tcase_add_test(tc_is_equal, is_equal_test16);

  tcase_add_test(tc_is_equal, is_equal_neg_test1);
  tcase_add_test(tc_is_equal, is_equal_neg_test2);
  tcase_add_test(tc_is_equal, is_equal_neg_test3);
  tcase_add_test(tc_is_equal, is_equal_neg_test4);
  tcase_add_test(tc_is_equal, is_equal_neg_test5);
  tcase_add_test(tc_is_equal, is_equal_neg_test6);
  tcase_add_test(tc_is_equal, is_equal_neg_test7);
  tcase_add_test(tc_is_equal, is_equal_neg_test8);
  tcase_add_test(tc_is_equal, is_equal_neg_test9);
  tcase_add_test(tc_is_equal, is_equal_neg_test10);
  tcase_add_test(tc_is_equal, is_equal_neg_test11);
  tcase_add_test(tc_is_equal, is_equal_neg_test12);
  tcase_add_test(tc_is_equal, is_equal_neg_test13);

  // ? +0 == -0 ?
  tcase_add_test(tc_is_equal, is_equal_null_test);
  tcase_add_test(tc_is_equal, is_equal_test_15);

  // ? доп тесты
  tcase_add_test(tc_is_equal, test_1);

  tcase_add_test(tc_is_equal, test_2);

  tcase_add_test(tc_is_equal, test_3);

  tcase_add_test(tc_is_equal, test_4);

  tcase_add_test(tc_is_equal, test_5);

  tcase_add_test(tc_is_equal, test_6);

  tcase_add_test(tc_is_equal, test_7);

  tcase_add_test(tc_is_equal, test_8);

  tcase_add_test(tc_is_equal, test_9);

  // ? hard test
  tcase_add_test(tc_is_equal, is_equal_hard1);

  suite_add_tcase(s, tc_is_equal);

  return s;
}