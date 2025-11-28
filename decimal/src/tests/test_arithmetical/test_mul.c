#include <check.h>

#include "../decimal_test.h"

// void test_mul__(s21_decimal decimal1, s21_decimal decimal2, s21_decimal
// check) {
//   s21_decimal result;
//   int code = s21_mul(decimal1, decimal2, &result);
//   ck_assert_int_eq(s21_is_equal(result, check), 1);
//   ck_assert_int_eq(code, NORMAL);
// }

// * Positive * Positive
START_TEST(mul_test1) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{50, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(mul_test2) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{200, 0, 0, 0}};
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{20000, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// * Negative * Negative
START_TEST(mul_test3) {
  s21_decimal a = {{5, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{10, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{50, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(mul_test4) {
  s21_decimal a = {{100, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{200, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{20000, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// * Positive * Negative
START_TEST(mul_test5) {
  s21_decimal a = {{5, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{50, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(mul_test6) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{200, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{20000, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// * Negative * Positive
START_TEST(mul_test7) {
  s21_decimal a = {{5, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{10, 0, 0, 0}};
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{50, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(mul_test8) {
  s21_decimal a = {{100, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{200, 0, 0, 0}};
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{20000, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// * Zero * Positive
START_TEST(mul_test9) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// * Positive * Zero
START_TEST(mul_test10) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// * Overflow
START_TEST(mul_test11) {
  s21_decimal a = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  s21_decimal b = {{2, 0, 0, 0}};
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, OVERFLOW);
}
END_TEST

// * Scale overflow
START_TEST(mul_test12) {
  s21_decimal a = {{5, 0, 0, 0}};
  set_decimal_scale(&a, 15);
  s21_decimal b = {{10, 0, 0, 0}};
  set_decimal_scale(&b, 15);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, UNDERFLOW);
}
END_TEST

// * Large numbers
START_TEST(mul_test13) {
  s21_decimal a = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// * Numbers with scale
START_TEST(mul_test14) {
  s21_decimal a = {{12345, 0, 0, 0}};
  set_decimal_scale(&a, 2);
  s21_decimal b = {{6789, 0, 0, 0}};
  set_decimal_scale(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{83810205, 0, 0, 0}};
  set_decimal_scale(&expected, 3);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// * тесты проверяющие переход разрядов
START_TEST(mul_test15) {
  s21_decimal a = {{UINT_MAX, 0, 0, 0}};
  s21_decimal b = {{2, 0, 0, 0}};
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{4294967294, 1, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(mul_test16) {
  s21_decimal a = {{UINT_MAX, 0, 0, 0}};
  s21_decimal b = {{UINT_MAX, 0, 0, 0}};
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{1, 4294967294, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// * Positive * Positive with scale
START_TEST(mul_test17) {
  s21_decimal a = {{5, 0, 0, 0}};
  set_decimal_scale(&a, 1);
  s21_decimal b = {{10, 0, 0, 0}};
  set_decimal_scale(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{5, 0, 0, 0}};
  set_decimal_scale(&expected, 1);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(mul_test18) {
  s21_decimal a = {{100, 0, 0, 0}};
  set_decimal_scale(&a, 2);
  s21_decimal b = {{200, 0, 0, 0}};
  set_decimal_scale(&b, 2);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{20000, 0, 0, 0}};
  set_decimal_scale(&expected, 4);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

// * Negative * Negative with scale
START_TEST(mul_test19) {
  s21_decimal a = {{5, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_scale(&a, 1);
  s21_decimal b = {{10, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  set_decimal_scale(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{50, 0, 0, 0}};
  set_decimal_scale(&expected, 2);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(mul_test20) {
  s21_decimal a = {{100, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_scale(&a, 2);
  s21_decimal b = {{200, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  set_decimal_scale(&b, 2);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{20000, 0, 0, 0}};
  set_decimal_scale(&expected, 4);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

// * Positive * Negative with scale
START_TEST(mul_test21) {
  s21_decimal a = {{5, 0, 0, 0}};
  set_decimal_scale(&a, 1);
  s21_decimal b = {{10, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  set_decimal_scale(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{50, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  set_decimal_scale(&expected, 2);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(mul_test22) {
  s21_decimal a = {{100, 0, 0, 0}};
  set_decimal_scale(&a, 2);
  s21_decimal b = {{200, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  set_decimal_scale(&b, 2);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{20000, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  set_decimal_scale(&expected, 4);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

// * Negative * Positive with scale
START_TEST(mul_test23) {
  s21_decimal a = {{5, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_scale(&a, 1);
  s21_decimal b = {{10, 0, 0, 0}};
  set_decimal_scale(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{50, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  set_decimal_scale(&expected, 2);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(mul_test24) {
  s21_decimal a = {{100, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_scale(&a, 2);
  s21_decimal b = {{200, 0, 0, 0}};
  set_decimal_scale(&b, 2);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{20000, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  set_decimal_scale(&expected, 4);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

// * Zero * Positive with scale
START_TEST(mul_test25) {
  s21_decimal a = {{0, 0, 0, 0}};
  set_decimal_scale(&a, 1);
  s21_decimal b = {{10, 0, 0, 0}};
  set_decimal_scale(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{0, 0, 0, 0}};
  set_decimal_scale(&expected, 2);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

// * Positive * Zero with scale
START_TEST(mul_test26) {
  s21_decimal a = {{10, 0, 0, 0}};
  set_decimal_scale(&a, 1);
  s21_decimal b = {{0, 0, 0, 0}};
  set_decimal_scale(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{0, 0, 0, 0}};
  set_decimal_scale(&expected, 2);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

// * Overflow with scale
START_TEST(mul_test27) {
  s21_decimal a = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  set_decimal_scale(&a, 1);
  s21_decimal b = {{2, 0, 0, 0}};
  set_decimal_scale(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, OVERFLOW);
}
END_TEST

// * Scale overflow with scale
START_TEST(mul_test28) {
  s21_decimal a = {{5, 0, 0, 0}};
  set_decimal_scale(&a, 15);
  s21_decimal b = {{10, 0, 0, 0}};
  set_decimal_scale(&b, 15);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, UNDERFLOW);
}
END_TEST

// * Large numbers with scale
START_TEST(mul_test29) {
  s21_decimal a = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  set_decimal_scale(&a, 1);
  s21_decimal b = {{1, 0, 0, 0}};
  set_decimal_scale(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  set_decimal_scale(&expected, 2);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

// * Numbers with scale
START_TEST(mul_test30) {
  s21_decimal a = {{12345, 0, 0, 0}};
  set_decimal_scale(&a, 2);
  s21_decimal b = {{6789, 0, 0, 0}};
  set_decimal_scale(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{83810205, 0, 0, 0}};
  set_decimal_scale(&expected, 3);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

// * тесты проверяющие переход разрядов with scale
START_TEST(mul_test31) {
  s21_decimal a = {{UINT_MAX, 0, 0, 0}};
  set_decimal_scale(&a, 1);
  s21_decimal b = {{2, 0, 0, 0}};
  set_decimal_scale(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{4294967294, 1, 0, 0}};
  set_decimal_scale(&expected, 2);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

START_TEST(mul_test32) {
  s21_decimal a = {{UINT_MAX, 0, 0, 0}};
  set_decimal_scale(&a, 1);
  s21_decimal b = {{UINT_MAX, 0, 0, 0}};
  set_decimal_scale(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{1, 4294967294, 0, 0}};
  set_decimal_scale(&expected, 2);
  ck_assert_int_eq(s21_is_equal(res, expected), 1);
}
END_TEST

// ? incorret decimal
START_TEST(mul_test33) {
  s21_decimal a = {{UINT_MAX, 0, 0, 0}};
  set_decimal_scale(&a, 29);
  s21_decimal b = {{UINT_MAX, 0, 0, 0}};
  set_decimal_scale(&b, 1);
  s21_decimal res;
  int status = s21_mul(a, b, &res);
  ck_assert_int_eq(status, UNDERFLOW);
}
END_TEST

START_TEST(test_mul_fail_manual11) {
  s21_decimal decimal2 = {{-1, 0, 0, 0x1D0000}};
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
  s21_decimal result;
  int code = s21_mul(decimal1, decimal2, &result);

  ck_assert_int_ne(code, NORMAL);
}
END_TEST

// START_TEST(test_mul_manual2) {
//   // 7.9228162514264337593543950335
//   s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
//   // -0.8228162514264337593543950335
//   s21_decimal decimal2 = {{0x4F63FFFF, 0xBB0D25CF, 0x1A962D2F, 0x801C0000}};
//   // -6.5190219687391279469416894926
//   s21_decimal check = {{0x23B23CAE, 0xEC60363A, 0xD2A415FA, 0x801C0000}};

//   test_mul__(decimal1, decimal2, check);
// }

// START_TEST(test_mul_manual3) {
//   // -7.000000000000025
//   s21_decimal decimal1 = {{0x816D8019, 0x18DE76, 0x0, 0x800F0000}};
//   // -0.00000000000005
//   s21_decimal decimal2 = {{0x5, 0x0, 0x0, 0x800E0000}};
//   // 0.0000000000003500000000000012
//   s21_decimal check = {{0x40B6C00C, 0xC6F3B, 0x0, 0x1C0000}};

//   test_mul__(decimal1, decimal2, check);
// }

Suite *test_mul(void) {
  Suite *s = suite_create("\033[45m S21_MUL \033[0m");
  TCase *tc_mul = tcase_create("tc_mul");

  tcase_add_test(tc_mul, mul_test1);
  tcase_add_test(tc_mul, mul_test2);
  tcase_add_test(tc_mul, mul_test3);
  tcase_add_test(tc_mul, mul_test4);
  tcase_add_test(tc_mul, mul_test5);
  tcase_add_test(tc_mul, mul_test6);
  tcase_add_test(tc_mul, mul_test7);
  tcase_add_test(tc_mul, mul_test8);
  tcase_add_test(tc_mul, mul_test9);
  tcase_add_test(tc_mul, mul_test10);
  tcase_add_test(tc_mul, mul_test11);
  tcase_add_test(tc_mul, mul_test12);
  tcase_add_test(tc_mul, mul_test13);
  tcase_add_test(tc_mul, mul_test14);
  // * тесты проверяющие переход разрядов
  tcase_add_test(tc_mul, mul_test15);
  tcase_add_test(tc_mul, mul_test16);
  // * тесты с числами с плавающей точкой
  tcase_add_test(tc_mul, mul_test17);
  tcase_add_test(tc_mul, mul_test18);
  tcase_add_test(tc_mul, mul_test19);
  tcase_add_test(tc_mul, mul_test20);
  tcase_add_test(tc_mul, mul_test21);
  tcase_add_test(tc_mul, mul_test22);
  tcase_add_test(tc_mul, mul_test23);
  tcase_add_test(tc_mul, mul_test24);
  tcase_add_test(tc_mul, mul_test25);
  tcase_add_test(tc_mul, mul_test26);
  tcase_add_test(tc_mul, mul_test27);
  tcase_add_test(tc_mul, mul_test28);
  tcase_add_test(tc_mul, mul_test29);
  tcase_add_test(tc_mul, mul_test30);
  tcase_add_test(tc_mul, mul_test31);
  tcase_add_test(tc_mul, mul_test32);
  // ? incorret decimal
  tcase_add_test(tc_mul, mul_test33);

  tcase_add_test(tc_mul, test_mul_fail_manual11);
  // tcase_add_test(tc_mul, test_mul_manual3);

  suite_add_tcase(s, tc_mul);
  return s;
}