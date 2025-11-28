#include "../decimal_test.h"

// * кейсы positive number + positive number

START_TEST(add_test1) {
  ///////////////////////////////

  s21_decimal a = {{725, 0, 0, 0}};
  s21_decimal b = {{3333251, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  s21_add(a, b, &res);
  s21_decimal expected = {{3333976, 0, 0, 0}};
  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(add_test2) {
  ///////////////////////////////

  s21_decimal a = {{UINT_MAX, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  s21_add(a, b, &res);
  s21_decimal expected = {{0, 1, 0, 0}};
  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(add_test3) {
  ///////////////////////////////

  s21_decimal a = {{UINT_MAX, UINT_MAX, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  s21_add(a, b, &res);
  s21_decimal expected = {{0, 0, 1, 0}};
  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(add_test4) {
  ///////////////////////////////

  s21_decimal a = {{725, 0, 0, 0}};
  set_decimal_scale(&a, 2);
  s21_decimal b = {{725, 0, 0, 0}};
  set_decimal_scale(&b, 2);
  s21_decimal res = {{0, 0, 0, 0}};
  s21_add(a, b, &res);
  s21_decimal expected = {{1450, 0, 0, 0}};
  set_decimal_scale(&expected, 2);
  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(add_test5) {
  ///////////////////////////////

  s21_decimal a = {{725, 0, 0, 0}};
  s21_decimal b = {{725, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  s21_add(a, b, &res);
  s21_decimal expected = {{1450, 0, 0, 0}};
  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// ? кейсы negative number + negative number

START_TEST(add_test6) {
  ///////////////////////////////

  s21_decimal a = {{725, 0, 0, 0}};
  s21_decimal b = {{3333251, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  s21_decimal res = {{0, 0, 0, 0}};
  s21_add(a, b, &res);
  s21_decimal expected = {{3333976, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(add_test7) {
  ///////////////////////////////
  s21_decimal a = {{UINT_MAX, 0, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  s21_add(a, b, &res);
  s21_decimal expected = {{0, 1, 0, 0}};
  set_decimal_sign(&expected, 1);
  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(add_test8) {
  ///////////////////////////////

  s21_decimal a = {{UINT_MAX, UINT_MAX, 0, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  s21_add(a, b, &res);
  s21_decimal expected = {{0, 0, 1, 0}};
  set_decimal_sign(&expected, 1);

  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(add_test9) {
  ///////////////////////////////

  s21_decimal a = {{725, 0, 0, 0}};
  set_decimal_scale(&a, 2);
  s21_decimal b = {{725, 0, 0, 0}};
  set_decimal_scale(&b, 2);
  s21_decimal res = {{0, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  s21_add(a, b, &res);
  s21_decimal expected = {{1450, 0, 0, 0}};
  set_decimal_scale(&expected, 2);
  set_decimal_sign(&expected, 1);

  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(add_test10) {
  ///////////////////////////////

  s21_decimal a = {{725, 0, 0, 0}};
  s21_decimal b = {{725, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  s21_decimal res = {{0, 0, 0, 0}};
  s21_add(a, b, &res);
  s21_decimal expected = {{1450, 0, 0, 0}};
  set_decimal_sign(&expected, 1);

  ///////////////////////////////
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// * кейсы with overflow
START_TEST(add_test11) {
  s21_decimal a = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal res;
  int status = s21_add(a, b, &res);
  ck_assert_int_eq(status, OVERFLOW);
}
END_TEST

// * кейсы with different signs
START_TEST(add_test12) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{50, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_add(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{50, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(add_test13) {
  s21_decimal a = {{50, 0, 0, 0}};
  s21_decimal b = {{100, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_add(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{50, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(add_test14) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_add(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(add_test15) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{100, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_add(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{90, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(add_test16) {
  s21_decimal a = {{0, 1, 0, 0}};
  s21_decimal b = {{INT_MAX, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_add(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{2147483649, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// ? дополнительные кейсы
// Сложение числа с нулем
START_TEST(add_test17) {
  s21_decimal a = {{725, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  s21_add(a, b, &res);
  s21_decimal expected = {{725, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Сложение чисел с разными знаками, где результат равен нулю
START_TEST(add_test18) {
  s21_decimal a = {{725, 0, 0, 0}};
  s21_decimal b = {{725, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res = {{0, 0, 0, 0}};
  s21_add(a, b, &res);
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Сложение чисел с разными масштабами
START_TEST(add_test19) {
  s21_decimal a = {{725, 0, 0, 0}};
  set_decimal_scale(&a, 1);
  s21_decimal b = {{725, 0, 0, 0}};
  set_decimal_scale(&b, 2);
  s21_decimal res = {{0, 0, 0, 0}};
  s21_add(a, b, &res);
  s21_decimal expected = {{7975, 0, 0, 0}};
  set_decimal_scale(&expected, 2);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Сложение чисел с максимальными значениями масштаба
START_TEST(add_test20) {
  s21_decimal a = {{1, 0, 0, 0}};
  set_decimal_scale(&a, 28);
  s21_decimal b = {{1, 0, 0, 0}};
  set_decimal_scale(&b, 28);
  s21_decimal res = {{0, 0, 0, 0}};
  s21_add(a, b, &res);
  s21_decimal expected = {{2, 0, 0, 0}};
  set_decimal_scale(&expected, 28);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Сложение чисел с максимальными значениями масштаба
START_TEST(add_test21) {
  s21_decimal a = {{1, 0, 0, 0}};
  set_decimal_scale(&a, 28);
  s21_decimal b = {{1, 0, 0, 0}};
  set_decimal_scale(&b, 28);
  s21_decimal res = {{0, 0, 0, 0}};
  s21_add(a, b, &res);
  s21_decimal expected = {{2, 0, 0, 0}};
  set_decimal_scale(&expected, 28);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

Suite *test_add(void) {
  Suite *s = suite_create("\033[45m S21_ADD \033[0m");
  TCase *tc_add = tcase_create("tc_add");
  // Дима, какие кейсы, MyCSGO??

  // * кейсы positive number + positive number
  tcase_add_test(tc_add, add_test1);
  tcase_add_test(tc_add, add_test2);
  tcase_add_test(tc_add, add_test3);
  tcase_add_test(tc_add, add_test4);
  tcase_add_test(tc_add, add_test5);
  // * кейсы negative number + negative number
  tcase_add_test(tc_add, add_test6);
  tcase_add_test(tc_add, add_test7);
  tcase_add_test(tc_add, add_test8);
  tcase_add_test(tc_add, add_test9);
  tcase_add_test(tc_add, add_test10);
  // * кейсы with overflow
  tcase_add_test(tc_add, add_test11);
  // * кейсы with negative number + negative number
  tcase_add_test(tc_add, add_test12);
  tcase_add_test(tc_add, add_test13);
  tcase_add_test(tc_add, add_test14);
  tcase_add_test(tc_add, add_test15);
  tcase_add_test(tc_add, add_test16);
  // * дополнительные кейсы
  tcase_add_test(tc_add, add_test17);
  tcase_add_test(tc_add, add_test18);
  tcase_add_test(tc_add, add_test19);
  tcase_add_test(tc_add, add_test20);
  tcase_add_test(tc_add, add_test21);

  suite_add_tcase(s, tc_add);
  return s;
}