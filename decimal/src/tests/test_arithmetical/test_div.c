#include "../decimal_test.h"

// * Positive / Positive
START_TEST(test_1) {
  s21_decimal a = {{50, 0, 0, 0}};
  s21_decimal b = {{5, 0, 0, 0}};
  s21_decimal res;
  int status = s21_div(a, b, &res);
  ck_assert_int_eq(status, NORMAL);
  s21_decimal expected = {{10, 0, 0, 0}};
  ck_assert_mem_eq(&res, &expected, sizeof(s21_decimal));
}
END_TEST
START_TEST(test_2) {
  s21_decimal value_1, value_2, result;

  value_1.bits[0] = 0b00000000000000000000000000000001;
  value_1.bits[1] = 0b00000000000000000000000000000000;
  value_1.bits[2] = 0b00000000000000000000000000000000;
  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000010;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b00000000000000000000000000000000;

  int r = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(r, 0);
  // printf("test2:");
  // print_dec(result);
}
END_TEST

START_TEST(test_3) {
  s21_decimal value_1, value_2, original, result;

  value_1.bits[0] = 0b11111111111111111111111111111111;
  value_1.bits[1] = 0b11111111111111111111111111111111;
  value_1.bits[2] = 0b00000000000000000000000000000000;
  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000010;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b00000000000000000000000000000000;
  int r = s21_div(value_1, value_2, &result);
  // printf("Test3:\n");
  // print_dec(result);
  ck_assert_int_eq(r, 0);
  original.bits[0] = 0b11111111111111111111111111111011;
  original.bits[1] = 0b11111111111111111111111111111111;
  original.bits[2] = 0b00000000000000000000000000000100;
  original.bits[3] = 0b00000000000000010000000000000000;
  // print_dec(original);

  ck_assert_int_eq(original.bits[3], result.bits[3]);
  ck_assert_int_eq(original.bits[2], result.bits[2]);
  ck_assert_int_eq(original.bits[1], result.bits[1]);
  ck_assert_int_eq(original.bits[0], result.bits[0]);
}
END_TEST

START_TEST(test_4) {
  s21_decimal value_1, value_2, original, result;

  value_1.bits[0] = 0b00000000000000000000000000000100;
  value_1.bits[1] = 0b00000000000000000000000000000000;
  value_1.bits[2] = 0b00000000000000000000000000000000;
  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000100;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(s21_div(value_1, value_2, &result), 0);

  original.bits[0] = 0b00000000000000000000000000000001;
  original.bits[1] = 0b00000000000000000000000000000000;
  original.bits[2] = 0b00000000000000000000000000000000;
  original.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(original.bits[3], result.bits[3]);
  ck_assert_int_eq(original.bits[2], result.bits[2]);
  ck_assert_int_eq(original.bits[1], result.bits[1]);
  ck_assert_int_eq(original.bits[0], result.bits[0]);
}
END_TEST

START_TEST(test_5) {
  s21_decimal value_1, value_2, original, result;

  value_1.bits[0] = 0b00000000000000000000000000010000;
  value_1.bits[1] = 0b00000000000000000000000000000000;
  value_1.bits[2] = 0b00000000000000000000000000000000;
  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000010;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b10000000000000010000000000000000;
  ck_assert_int_eq(s21_div(value_1, value_2, &result), 0);

  original.bits[0] = 0b00000000000000000000000001010000;
  original.bits[1] = 0b00000000000000000000000000000000;
  original.bits[2] = 0b00000000000000000000000000000000;
  original.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(original.bits[3], result.bits[3]);
  ck_assert_int_eq(original.bits[2], result.bits[2]);
  ck_assert_int_eq(original.bits[1], result.bits[1]);
  ck_assert_int_eq(original.bits[0], result.bits[0]);
}
END_TEST

// Биты от 0 до 15 не используются и должны быть равны 0
START_TEST(test_6) {
  s21_decimal value_1, value_2, result;

  value_1.bits[0] = 0b00000000000000000000000000000000;
  value_1.bits[1] = 0b00000000000000000000000000000000;
  value_1.bits[2] = 0b00000000000000000000000000000000;
  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000000;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b00000000000000000000000000000011;
  ck_assert_int_eq(s21_div(value_1, value_2, &result), 1);
}
END_TEST

// Биты с 24 по 30 не используются и должны быть равны нулю.
START_TEST(test_7) {
  s21_decimal value_1, value_2, result;

  value_1.bits[0] = 0b00000000000000000000000000000000;
  value_1.bits[1] = 0b00000000000000000000000000000000;
  value_1.bits[2] = 0b00000000000000000000000000000000;
  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000000;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b01111000000000000000000000000000;
  ck_assert_int_eq(s21_div(value_1, value_2, &result), 1);
}
END_TEST

// Биты с 16 по 23 должны содержать показатель степени от 0 до 28
START_TEST(test_8) {
  s21_decimal value_1, value_2, result;

  value_1.bits[0] = 0b00000000000000000000000000000000;
  value_1.bits[1] = 0b00000000000000000000000000000000;
  value_1.bits[2] = 0b00000000000000000000000000000000;
  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000000;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b00000000011111111000000000000000;
  ck_assert_int_eq(s21_div(value_1, value_2, &result), 1);
}
END_TEST

// 3 деление на 0
START_TEST(test_9) {
  s21_decimal value_1, value_2, original, result;

  value_1.bits[0] = 0b00000000000000000000000000000010;
  value_1.bits[1] = 0b00000000000000000000000000000000;
  value_1.bits[2] = 0b00000000000000000000000000000000;
  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000000;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(s21_div(value_1, value_2, &result), 3);

  original.bits[0] = 0b00000000000000000000000000000000;
  original.bits[1] = 0b00000000000000000000000000000000;
  original.bits[2] = 0b00000000000000000000000000000000;
  original.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(original.bits[3], result.bits[3]);
  ck_assert_int_eq(original.bits[2], result.bits[2]);
  ck_assert_int_eq(original.bits[1], result.bits[1]);
  ck_assert_int_eq(original.bits[0], result.bits[0]);
}
END_TEST

START_TEST(test_10) {
  s21_decimal value_1, value_2, original, result;

  value_1.bits[0] = 0b00000000000000000000000000000000;
  value_1.bits[1] = 0b00000000000000000000000000000000;
  value_1.bits[2] = 0b00000000000000000000000000000001;
  value_1.bits[3] = 0b10000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000011111111;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(s21_div(value_1, value_2, &result), 0);

  original.bits[0] = 0b01110011011100110111001101110011;
  original.bits[1] = 0b01110011011100110111001101110011;
  original.bits[2] = 0b11101001101111100110001101110011;
  original.bits[3] = 0b10000000000011000000000000000000;
  ck_assert_int_eq(original.bits[3], result.bits[3]);
  ck_assert_int_eq(original.bits[2], result.bits[2]);
  ck_assert_int_eq(original.bits[1], result.bits[1]);
  ck_assert_int_eq(original.bits[0], result.bits[0]);
}
END_TEST

START_TEST(test_11) {
  s21_decimal value_1, value_2, original, result;

  value_1.bits[0] = 0b11111111111111111111111111111111;
  value_1.bits[1] = 0b11111111111111111111111111111111;
  value_1.bits[2] = 0b11111111111111111111111111111111;
  value_1.bits[3] = 0b10000000000000000000000000000000;

  value_2.bits[0] = 0b11111111111111111111111111111111;
  value_2.bits[1] = 0b11111111111111111111111111111111;
  value_2.bits[2] = 0b11111111111111111111111111111111;
  value_2.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(s21_div(value_1, value_2, &result), 0);

  original.bits[0] = 0b00000000000000000000000000000001;
  original.bits[1] = 0b00000000000000000000000000000000;
  original.bits[2] = 0b00000000000000000000000000000000;
  original.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(original.bits[3], result.bits[3]);
  ck_assert_int_eq(original.bits[2], result.bits[2]);
  ck_assert_int_eq(original.bits[1], result.bits[1]);
  ck_assert_int_eq(original.bits[0], result.bits[0]);
}
END_TEST
START_TEST(test_12) {
  s21_decimal value_1, value_2, original, result;

  value_1.bits[0] = 0b00000000000000000000000001011100;
  value_1.bits[1] = 0b00000000000000000000000000000000;
  value_1.bits[2] = 0b00000000000000000000000000000000;
  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000110;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b10000000000000100000000000000000;
  ck_assert_int_eq(s21_div(value_1, value_2, &result), 0);

  original.bits[0] = 0b11100101010101010101010101010101;
  original.bits[1] = 0b10010010011111010000001110100101;
  original.bits[2] = 0b00110001100010110110111110100001;
  original.bits[3] = 0b10000000000110010000000000000000;
  ck_assert_int_eq(original.bits[3], result.bits[3]);
  ck_assert_int_eq(original.bits[2], result.bits[2]);
  ck_assert_int_eq(original.bits[1], result.bits[1]);
  ck_assert_int_eq(original.bits[0], result.bits[0]);
}
END_TEST

START_TEST(test_13) {
  s21_decimal value_1, value_2, result;

  value_1.bits[0] = 0b00000000000000000000000000000010;
  value_1.bits[1] = 0b00000000000000000000000000000000;
  value_1.bits[2] = 0b00000000000000000000000000000000;
  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000010;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(s21_div(value_1, value_2, &result), 0);
}
END_TEST

START_TEST(test_14) {
  s21_decimal value_1, value_2, original, result;

  value_1.bits[0] = 0b00000000000000000000000000000000;
  value_1.bits[1] = 0b00000000000000000000000000000000;
  value_1.bits[2] = 0b00000000000000000000000000000001;
  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000001010;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(s21_div(value_1, value_2, &result), 0);

  original.bits[0] = 0b00000000000000000000000000000000;
  original.bits[1] = 0b00000000000000000000000000000000;
  original.bits[2] = 0b00000000000000000000000000000001;
  original.bits[3] = 0b10000000000000010000000000000000;
  ck_assert_int_eq(original.bits[3], result.bits[3]);
  ck_assert_int_eq(original.bits[2], result.bits[2]);
  ck_assert_int_eq(original.bits[1], result.bits[1]);
  ck_assert_int_eq(original.bits[0], result.bits[0]);
}
END_TEST

START_TEST(test_15) {
  s21_decimal value_1, value_2, original, result;

  value_1.bits[0] = 0b00000000000000000000000000000010;
  value_1.bits[1] = 0b00000000000000000000000000000000;
  value_1.bits[2] = 0b00000000000000000000000000000000;
  value_1.bits[3] = 0b10000000000000010000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000010;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b00000000000000100000000000000000;
  ck_assert_int_eq(s21_div(value_1, value_2, &result), 0);

  original.bits[0] = 0b00000000000000000000000000001010;
  original.bits[1] = 0b00000000000000000000000000000000;
  original.bits[2] = 0b00000000000000000000000000000000;
  original.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(original.bits[3], result.bits[3]);
  ck_assert_int_eq(original.bits[2], result.bits[2]);
  ck_assert_int_eq(original.bits[1], result.bits[1]);
  ck_assert_int_eq(original.bits[0], result.bits[0]);
}
END_TEST

// START_TEST(test_16) {
//   s21_decimal value_1, value_2, original, result;

//   value_1.bits[0] = 0b10101011011100010101010101010101;
//   value_1.bits[1] = 0b11010101111010101010101010101011;
//   value_1.bits[2] = 0b00000000000000000000000000000011;
//   value_1.bits[3] = 0b10000000000001110000000000000000;

//   value_2.bits[0] = 0b00000000101010101010101110111111;
//   value_2.bits[1] = 0b00000000000000000000000000000000;
//   value_2.bits[2] = 0b00000000000000000000000000000000;
//   value_2.bits[3] = 0b00000000000101110000000000000000;
//   ck_assert_int_eq(s21_div(value_1, value_2, &result), 0);

//   original.bits[0] = 0b00011111100111111111010100010100;
//   original.bits[1] = 0b00110010001111011000101101100101;
//   original.bits[2] = 0b11001100011001011011110100100100;
//   original.bits[3] = 0b10000000000000000000000000000000;
//   ck_assert_int_eq(original.bits[3], result.bits[3]);
//   ck_assert_int_eq(original.bits[2], result.bits[2]);
//   ck_assert_int_eq(original.bits[1], result.bits[1]);
//   ck_assert_int_eq(original.bits[0], result.bits[0]);
// }
// END_TEST

START_TEST(decimal_test_div_1) {
  s21_decimal num1 = {{0, 0, 0, 0}};
  s21_decimal num2 = {{0, 0, 0, 0}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_uint_eq(num3.bits[0], 0);
  ck_assert_uint_eq(num3.bits[1], 0);
  ck_assert_uint_eq(num3.bits[2], 0);
  ck_assert_uint_eq(num3.bits[3], 0);
  ck_assert_int_eq(error, 3);
}
END_TEST
START_TEST(decimal_test_div_2) {
  s21_decimal num1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0}};
  s21_decimal num2 = {{0b1010, 0, 0, 0}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_uint_eq(num3.bits[0], 0xffffffff);
  ck_assert_uint_eq(num3.bits[1], 0xffffffff);
  ck_assert_uint_eq(num3.bits[2], 0xffffffff);
  ck_assert_uint_eq(num3.bits[3], 0x00010000);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(decimal_test_div_3) {
  s21_decimal num1 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0}};
  s21_decimal num2 = {{1, 0, 0, 0}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_uint_eq(num3.bits[0], UINT32_MAX);
  ck_assert_uint_eq(num3.bits[1], UINT32_MAX);
  ck_assert_uint_eq(num3.bits[2], UINT32_MAX);
  ck_assert_uint_eq(num3.bits[3], 0);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(decimal_test_div_4) {
  s21_decimal num1 = {{0, 0, 0, 0}};
  s21_decimal num2 = {{1, 1, 1, 0}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_uint_eq(num3.bits[0], 0);
  ck_assert_uint_eq(num3.bits[1], 0);
  ck_assert_uint_eq(num3.bits[2], 0);
  ck_assert_uint_eq(num3.bits[3], 0);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(decimal_test_div_5) {
  s21_decimal num1 = {{1, 0, 0, 0x00000000}};
  s21_decimal num2 = {{1, 0, 0, 0x00020000}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_uint_eq(num3.bits[0], 100);
  ck_assert_uint_eq(num3.bits[1], 0);
  ck_assert_uint_eq(num3.bits[2], 0);
  ck_assert_uint_eq(num3.bits[3], 0);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(decimal_test_div_6) {
  s21_decimal num1 = {{0xffffffff, 0xffffffff, 0, 0x00030000}};
  s21_decimal num2 = {{1, 0, 0, 0}};
  s21_decimal num3 = {{0}};

  int error = s21_div(num1, num2, &num3);
  ck_assert_uint_eq(num3.bits[0], 0xffffffff);
  ck_assert_uint_eq(num3.bits[1], 0xffffffff);
  ck_assert_uint_eq(num3.bits[2], 0);
  ck_assert_uint_eq(num3.bits[3], 0x00030000);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(decimal_test_div_7) {
  s21_decimal num1 = {{0xffffffff, 0xffffffff, 0xffffffff, 0x00030000}};
  s21_decimal num2 = {{1, 0, 0, 0x00030000}};
  s21_decimal num3 = {{0}};

  int error = s21_div(num1, num2, &num3);
  ck_assert_uint_eq(num3.bits[0], 0xffffffff);
  ck_assert_uint_eq(num3.bits[1], 0xffffffff);
  ck_assert_uint_eq(num3.bits[2], 0xffffffff);
  ck_assert_uint_eq(num3.bits[3], 0);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(decimal_test_div_8) {
  s21_decimal num1 = {{1000, 1000, 1000, 0x00030000}};
  s21_decimal num2 = {{1, 1, 1, 0x80000000}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_uint_eq(num3.bits[0], 1);
  ck_assert_uint_eq(num3.bits[1], 0);
  ck_assert_uint_eq(num3.bits[2], 0);
  ck_assert_uint_eq(num3.bits[3], 0x80000000);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(decimal_test_div_9) {
  s21_decimal num1 = {{1000, 1000, 1000, 0x00000000}};
  s21_decimal num2 = {{1000, 1000, 1000, 0x80000000}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_uint_eq(num3.bits[0], 1);
  ck_assert_uint_eq(num3.bits[1], 0);
  ck_assert_uint_eq(num3.bits[2], 0);
  ck_assert_uint_eq(num3.bits[3], 0x80000000);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(decimal_test_div_10) {
  s21_decimal num1 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0x80000000}};
  s21_decimal num2 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0x80000000}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_uint_eq(num3.bits[0], 1);
  ck_assert_uint_eq(num3.bits[1], 0);
  ck_assert_uint_eq(num3.bits[2], 0);
  ck_assert_uint_eq(num3.bits[3], 0x00000000);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(decimal_test_div_11) {
  s21_decimal num1 = {{1000, 1000, 1000, 0x001E0000}};
  s21_decimal num2 = {{1000, 1000, 1000, 0x80010000}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_uint_eq(num3.bits[0], 0);
  ck_assert_uint_eq(num3.bits[1], 0);
  ck_assert_uint_eq(num3.bits[2], 0);
  ck_assert_uint_eq(num3.bits[3], 0x00000000);
  ck_assert_int_eq(error, 1);
}
START_TEST(decimal_test_div_12) {
  s21_decimal num1 = {{1000, 1000, 1000, 0x00010000}};
  s21_decimal num2 = {{1000, 1000, 1000, 0x801E0000}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_uint_eq(num3.bits[0], 0);
  ck_assert_uint_eq(num3.bits[1], 0);
  ck_assert_uint_eq(num3.bits[2], 0);
  ck_assert_uint_eq(num3.bits[3], 0x00000000);
  ck_assert_int_eq(error, 1);
}
START_TEST(decimal_test_div_13) {
  s21_decimal num1 = {{1000, 1000, 1000, 0x00000001}};
  s21_decimal num2 = {{1000, 1000, 1000, 0x80000000}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_uint_eq(num3.bits[0], 0);
  ck_assert_uint_eq(num3.bits[1], 0);
  ck_assert_uint_eq(num3.bits[2], 0);
  ck_assert_uint_eq(num3.bits[3], 0x00000000);
  ck_assert_int_eq(error, 1);
}
START_TEST(decimal_test_div_14) {
  s21_decimal num1 = {{1000, 1000, 1000, 0x00000000}};
  s21_decimal num2 = {{1000, 1000, 1000, 0x80000001}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_uint_eq(num3.bits[0], 0);
  ck_assert_uint_eq(num3.bits[1], 0);
  ck_assert_uint_eq(num3.bits[2], 0);
  ck_assert_uint_eq(num3.bits[3], 0x00000000);
  ck_assert_int_eq(error, 1);
}
END_TEST
START_TEST(decimal_test_div_15) {
  s21_decimal num1 = {{1000, 1000, 1000, 0x00000000}};
  s21_decimal num2 = {{1000, 1000, 1000, 0x80000000}};
  // s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, NULL);
  ck_assert_int_eq(error, 1);
}
END_TEST
START_TEST(decimal_test_div_16) {
  s21_decimal num1 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0x001E0000}};
  s21_decimal num2 = {{1, 1, 1, 0x00000000}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_int_eq(error, 1);
}
END_TEST
START_TEST(decimal_test_div_17) {
  s21_decimal num1 = {{UINT32_MAX, UINT32_MAX, UINT32_MAX, 0x00000000}};
  s21_decimal num2 = {{1, 0, 0, 0x801c0000}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_int_eq(error, 2);
}
END_TEST
START_TEST(decimal_test_div_18) {
  s21_decimal num1 = {{1000, 1000, 1000, 0x80000000}};
  s21_decimal num2 = {{500, 500, 500, 0x00000000}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(decimal_test_div_19) {
  s21_decimal num1 = {{500, 500, 500, 0x80000000}};
  s21_decimal num2 = {{1000, 1000, 1000, 0x00000000}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(decimal_test_div_20) {
  s21_decimal num1 = {{500, 500, 500, 0x00000000}};
  s21_decimal num2 = {{1000, 1000, 1000, 0x80000000}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(decimal_test_div_periodic) {
  s21_decimal num1 = {{1, 0, 0, 0}};
  s21_decimal num2 = {{3, 0, 0, 0}};
  s21_decimal num3 = {{0}};
  int error = s21_div(num1, num2, &num3);
#ifdef DEBUG
  printf("test_div_periodic:\n");
  print_dec(num1);
  print_dec(num2);
  print_dec(num3);
#endif

  ck_assert_int_eq(error, 0);
  ck_assert_msg(s21_is_equal(num3, (s21_decimal){{89478485, 347537611,
                                                  180700362, 1835008}}),
                "s21_equal decimal_test_div_periodic failed");
}
END_TEST

Suite* test_div(void) {
  Suite* suite = suite_create("\033[42m\033[30m========= DIV =========\033[0m");
  TCase* tcase_core = tcase_create("div");
  tcase_add_test(tcase_core, test_1);
  tcase_add_test(tcase_core, test_2);
  tcase_add_test(tcase_core, test_3);
  tcase_add_test(tcase_core, test_4);
  tcase_add_test(tcase_core, test_5);
  tcase_add_test(tcase_core, test_6);
  tcase_add_test(tcase_core, test_7);
  tcase_add_test(tcase_core, test_8);
  tcase_add_test(tcase_core, test_9);
  tcase_add_test(tcase_core, test_10);
  tcase_add_test(tcase_core, test_11);
  tcase_add_test(tcase_core, test_12);
  tcase_add_test(tcase_core, test_13);
  tcase_add_test(tcase_core, test_14);
  tcase_add_test(tcase_core, test_15);
  // tcase_add_test(tcase_core, test_16);
  tcase_add_test(tcase_core, decimal_test_div_1);
  tcase_add_test(tcase_core, decimal_test_div_2);
  tcase_add_test(tcase_core, decimal_test_div_3);
  tcase_add_test(tcase_core, decimal_test_div_4);
  tcase_add_test(tcase_core, decimal_test_div_5);
  tcase_add_test(tcase_core, decimal_test_div_6);
  tcase_add_test(tcase_core, decimal_test_div_7);
  tcase_add_test(tcase_core, decimal_test_div_8);
  tcase_add_test(tcase_core, decimal_test_div_9);
  tcase_add_test(tcase_core, decimal_test_div_10);
  tcase_add_test(tcase_core, decimal_test_div_11);
  tcase_add_test(tcase_core, decimal_test_div_12);
  tcase_add_test(tcase_core, decimal_test_div_13);
  tcase_add_test(tcase_core, decimal_test_div_14);
  tcase_add_test(tcase_core, decimal_test_div_15);
  tcase_add_test(tcase_core, decimal_test_div_16);
  tcase_add_test(tcase_core, decimal_test_div_17);
  tcase_add_test(tcase_core, decimal_test_div_18);
  tcase_add_test(tcase_core, decimal_test_div_19);
  tcase_add_test(tcase_core, decimal_test_div_20);
  tcase_add_test(tcase_core, decimal_test_div_periodic);
  suite_add_tcase(suite, tcase_core);
  return suite;
}