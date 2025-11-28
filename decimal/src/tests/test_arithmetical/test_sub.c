#include "../decimal_test.h"

// * кейсы positive number - positive number

START_TEST(sub_test1) {
  s21_decimal a = {{3333976, 0, 0, 0}};
  s21_decimal b = {{3333251, 0, 0, 0}};
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{725, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(sub_test2) {
  s21_decimal a = {{1, 0, 1, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{0, 0, 1, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(sub_test3) {
  s21_decimal a = {{725, 0, 0, 0}};
  set_decimal_scale(&a, 2);
  s21_decimal b = {{725, 0, 0, 0}};
  set_decimal_scale(&b, 2);
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(sub_test4) {
  s21_decimal a = {{1450, 0, 0, 0}};
  s21_decimal b = {{725, 0, 0, 0}};
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{725, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// * кейсы negative number - negative number
START_TEST(sub_test5) {
  s21_decimal a = {{3333976, 0, 0, 0}};
  s21_decimal b = {{3333251, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{725, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(sub_test6) {
  s21_decimal a = {{0, 0, 1, 0}};
  s21_decimal b = {{1, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{UINT_MAX, UINT_MAX, 0, 0}};
  set_decimal_sign(&expected, 1);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(sub_test7) {
  s21_decimal a = {{725, 0, 0, 0}};
  s21_decimal b = {{725, 0, 0, 0}};
  set_decimal_scale(&a, 2);  // 7.25
  set_decimal_scale(&b, 2);  // 7.25
  set_decimal_sign(&a, 1);   // -7.25
  set_decimal_sign(&b, 1);   // -7.25
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(sub_test8) {
  s21_decimal a = {{1450, 0, 0, 0}};
  s21_decimal b = {{725, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{725, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// * кейсы with overflow
START_TEST(sub_test9) {
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
}
END_TEST

// * кейсы with underflow
START_TEST(sub_test10) {
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{UINT_MAX, UINT_MAX, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
}
END_TEST

// * кейсы with different signs
START_TEST(sub_test11) {
  s21_decimal a = {{100, 0, 0, 0}};
  s21_decimal b = {{50, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{150, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(sub_test12) {
  s21_decimal a = {{50, 0, 0, 0}};
  s21_decimal b = {{100, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{150, 0, 0, 0}};
  set_decimal_sign(&expected, 0);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// дополнительные тесты

// Вычитание числа из самого себя
START_TEST(sub_test14) {
  s21_decimal a = {{725, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  s21_sub(a, a, &res);
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Вычитание числа из нуля
START_TEST(sub_test15) {
  s21_decimal a = {{725, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  s21_sub(b, a, &res);
  s21_decimal expected = {{725, 0, 0, 0}};
  set_decimal_sign(&expected, 1);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Вычитание нуля из числа
START_TEST(sub_test16) {
  s21_decimal a = {{725, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  s21_sub(a, b, &res);
  s21_decimal expected = {{725, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Вычитание чисел с разными знаками
START_TEST(sub_test17) {
  s21_decimal a = {{725, 0, 0, 0}};
  s21_decimal b = {{3333251, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res = {{0, 0, 0, 0}};
  s21_sub(a, b, &res);
  s21_decimal expected = {{3333976, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Вычитание чисел с одинаковыми знаками
START_TEST(sub_test18) {
  s21_decimal a = {{3333251, 0, 0, 0}};
  s21_decimal b = {{725, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  s21_sub(a, b, &res);
  s21_decimal expected = {{3332526, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Вычитание чисел с разными масштабами
START_TEST(sub_test19) {
  s21_decimal a = {{725, 0, 0, 0}};
  set_decimal_scale(&a, 1);
  s21_decimal b = {{725, 0, 0, 0}};
  set_decimal_scale(&b, 2);
  s21_decimal res = {{0, 0, 0, 0}};
  s21_sub(a, b, &res);
  s21_decimal expected = {{6525, 0, 0, 0}};
  set_decimal_scale(&expected, 2);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Вычитание чисел с максимальными значениями масштаба
START_TEST(sub_test20) {
  s21_decimal a = {{1, 0, 0, 0}};
  set_decimal_scale(&a, 28);
  s21_decimal b = {{1, 0, 0, 0}};
  set_decimal_scale(&b, 28);
  s21_decimal res = {{0, 0, 0, 0}};
  s21_sub(a, b, &res);
  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Вычитание чисел с переполнением масштаба
START_TEST(sub_test21) {
  s21_decimal a = {{1, 0, 0, 0}};
  set_decimal_scale(&a, 28);
  s21_decimal b = {{1, 0, 0, 0}};
  set_decimal_scale(&b, 28);
  s21_decimal res = {{0, 0, 0, 0}};
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
}
END_TEST

// Вычитание чисел с разными знаками, где результат не равен нулю
START_TEST(sub_test22) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{10, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{20, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Вычитание чисел с разными знаками, где результат не равен нулю
START_TEST(sub_test23) {
  s21_decimal a = {{10, 0, 0, 0}};
  s21_decimal b = {{100, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{110, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Вычитание числа из большего числа
START_TEST(sub_test24) {
  s21_decimal a = {{0, 1, 0, 0}};
  s21_decimal b = {{INT_MAX, 0, 0, 0}};
  s21_decimal res;
  int status = s21_sub(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{2147483649, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Вычитание числа с нулем
START_TEST(sub_test25) {
  s21_decimal a = {{725, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  s21_decimal res = {{0, 0, 0, 0}};
  s21_sub(a, b, &res);
  s21_decimal expected = {{725, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Вычитание чисел с разными знаками, где результат равен нулю
START_TEST(sub_test26) {
  s21_decimal a = {{725, 0, 0, 0}};
  s21_decimal b = {{725, 0, 0, 0}};
  set_decimal_sign(&b, 1);
  s21_decimal res = {{0, 0, 0, 0}};
  s21_sub(a, b, &res);
  s21_decimal expected = {{1450, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// Вычитание чисел с разными масштабами
START_TEST(sub_test27) {
  s21_decimal a = {{725, 0, 0, 0}};
  set_decimal_scale(&a, 1);
  s21_decimal b = {{725, 0, 0, 0}};
  set_decimal_scale(&b, 2);
  s21_decimal res = {{0, 0, 0, 0}};
  s21_sub(a, b, &res);
  s21_decimal expected = {{6525, 0, 0, 0}};
  set_decimal_scale(&expected, 2);
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST

// START_TEST(sub_test28) {
//   ///////////////////////////////
//   s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
//   set_decimal_scale(&a, 3);
//   s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
//   set_decimal_scale(&b, 2);
//   s21_decimal result;
//   s21_sub(a, b, &result);
//   s21_decimal expected = {{0b10110110100111101000010110000001,
//                            0b10101001111110111110011010000011,
//                            0b00000010010011011101001011110001, 0}};
//   set_decimal_sign(&expected, 1);
//   ///////////////////////////////
//   ck_assert_int_eq(s21_is_equal(result, expected), 1);
// }
// END_TEST

Suite *test_sub(void) {
  Suite *s = suite_create("\033[45m S21_SUB \033[0m");
  TCase *tc_sub = tcase_create("tc_sub");

  // * кейсы positive number - positive number
  tcase_add_test(tc_sub, sub_test1);
  tcase_add_test(tc_sub, sub_test2);
  tcase_add_test(tc_sub, sub_test3);
  tcase_add_test(tc_sub, sub_test4);
  // * кейсы negative number - negative number
  tcase_add_test(tc_sub, sub_test5);
  tcase_add_test(tc_sub, sub_test6);
  tcase_add_test(tc_sub, sub_test7);
  tcase_add_test(tc_sub, sub_test8);
  // * кейсы with overflow
  tcase_add_test(tc_sub, sub_test9);
  // * кейсы with underflow
  tcase_add_test(tc_sub, sub_test10);
  // * кейсы with different signs
  tcase_add_test(tc_sub, sub_test11);
  tcase_add_test(tc_sub, sub_test12);
  // ? дополнительные тесты
  tcase_add_test(tc_sub, sub_test14);
  tcase_add_test(tc_sub, sub_test15);
  tcase_add_test(tc_sub, sub_test16);
  tcase_add_test(tc_sub, sub_test17);
  tcase_add_test(tc_sub, sub_test18);
  tcase_add_test(tc_sub, sub_test19);
  tcase_add_test(tc_sub, sub_test20);
  tcase_add_test(tc_sub, sub_test21);
  tcase_add_test(tc_sub, sub_test22);
  tcase_add_test(tc_sub, sub_test23);
  tcase_add_test(tc_sub, sub_test24);
  tcase_add_test(tc_sub, sub_test25);
  tcase_add_test(tc_sub, sub_test26);
  tcase_add_test(tc_sub, sub_test27);
  // ? hard tests
  // tcase_add_test(tc_sub, sub_test28);

  suite_add_tcase(s, tc_sub);
  return s;
}