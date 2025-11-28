#include "../decimal_test.h"

START_TEST(is_less_test1) {
  ///////////////////////////////
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_test2) {
  ///////////////////////////////
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{0, 1, 0, 0}};
  int result = s21_is_less(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_test3) {
  ///////////////////////////////
  s21_decimal a = {{1, 0, 1, 0}};
  s21_decimal b = {{1, 1, 0, 0}};
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_test4) {
  ///////////////////////////////
  s21_decimal a = {{0, 0, 1, 0}};
  s21_decimal b = {{0, INT_MAX, 0, 0}};
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_test5) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  s21_decimal b = {{INT_MAX, INT_MAX, 1, 0}};
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_test6) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&b, 1);
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_test7) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&b, 1);
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_test8) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  int result = s21_is_less(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_test9) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_test10) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 3);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  int result = s21_is_less(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_test11) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 2);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_test12) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 28);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 29);
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_test13) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 14);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 13);
  int result = s21_is_less(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

// ! negative tests

START_TEST(is_less_neg_test1) {
  ///////////////////////////////
  s21_decimal a = {{0, 0, 0, 0}};
  s21_decimal b = {{0, 0, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_neg_test2) {
  ///////////////////////////////
  s21_decimal a = {{1, 0, 0, 0}};
  s21_decimal b = {{0, 1, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_neg_test3) {
  ///////////////////////////////
  s21_decimal a = {{1, 0, 1, 0}};
  s21_decimal b = {{1, 1, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_less(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_neg_test4) {
  ///////////////////////////////
  s21_decimal a = {{0, 0, 1, 0}};
  s21_decimal b = {{0, INT_MAX, 0, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_less(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_neg_test5) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  s21_decimal b = {{INT_MAX, INT_MAX, 1, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_less(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_neg_test6) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  set_decimal_sign(&b, 1);
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_neg_test7) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&b, 1);
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_neg_test8) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  int result = s21_is_less(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_neg_test9) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 1);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  set_decimal_sign(&b, 1);
  int result = s21_is_less(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_neg_test10) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 3);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  set_decimal_sign(&b, 1);
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_neg_test11) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 2);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 2);
  set_decimal_sign(&b, 1);
  int result = s21_is_less(a, b);
  int expected = 0;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_neg_test12) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 28);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 29);
  set_decimal_sign(&b, 1);
  int result = s21_is_less(a, b);
  int expected = 1;
  ///////////////////////////////
  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(is_less_neg_test13) {
  ///////////////////////////////
  s21_decimal a = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&a, 14);
  set_decimal_sign(&a, 1);
  s21_decimal b = {{INT_MAX, INT_MAX, INT_MAX, 0}};
  set_decimal_scale(&b, 13);
  set_decimal_sign(&b, 1);
  int result = s21_is_less(a, b);
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

  ck_assert_int_eq(s21_is_less(value_1, value_2), 1);
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

  ck_assert_int_eq(s21_is_less(value_1, value_2), 1);
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

  ck_assert_int_eq(s21_is_less(value_1, value_2), 0);
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

  ck_assert_int_eq(s21_is_less(value_1, value_2), 1);
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

  ck_assert_int_eq(s21_is_less(value_1, value_2), 0);
}

END_TEST

START_TEST(test_6) {
  s21_decimal value_1, value_2;

  value_1.bits[0] = 0b00000000000000000000000000000000;

  value_1.bits[1] = 0b00000000000000000000000000000000;

  value_1.bits[2] = 0b00100000000000000000000000000000;

  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000010;

  value_2.bits[1] = 0b00000000000000000000000000000000;

  value_2.bits[2] = 0b01000000000000000000000000000000;

  value_2.bits[3] = 0b00000000000000000000000000000000;

  ck_assert_int_eq(s21_is_less(value_1, value_2), 1);
}

END_TEST

START_TEST(test_7) {
  s21_decimal value_1, value_2;

  value_1.bits[0] = 0b00000000000000000000000000000010;

  value_1.bits[1] = 0b00000000000000000000000000000001;

  value_1.bits[2] = 0b00000000000000000000000000000000;

  value_1.bits[3] = 0b00000000000000000000000000000000;

  value_2.bits[0] = 0b00000000000000000000000000000010;

  value_2.bits[1] = 0b00000000000000000000000000000001;

  value_2.bits[2] = 0b00000000000000000000000000000000;

  value_2.bits[3] = 0b00000000000000000000000000000000;

  ck_assert_int_eq(s21_is_less(value_1, value_2), 0);
}
END_TEST

START_TEST(is_less_hard1) {
  s21_decimal value_1, value_2;
  clean_decimal(&value_1);
  clean_decimal(&value_2);

  value_1.bits[0] = 0b01110001111110110000100001000011;

  value_1.bits[1] = 0b00000000000000000000000100011111;

  set_decimal_scale(&value_1, 12);

  value_2.bits[0] = 0b01110011110011100101001010011110;

  value_2.bits[1] = 0b00000000000000000000101100111010;

  set_decimal_scale(&value_2, 14);

  ck_assert_int_eq(s21_is_less(value_1, value_2), 0);
}

END_TEST

START_TEST(is_less_hard2) {
  ///////////////////////////////
  char enter_val_1[33] = "1.234567891011";
  char enter_val_2[33] = "0.12345678910110";
  int expectation = FALSE;

  char res_1[33];
  char res_2[33];
  ///// string to decimal
  s21_decimal val_dec_1 = {0};
  s21_decimal val_dec_2 = {0};
  binary_to_bits(
      decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
      &val_dec_1, enter_val_1);
  binary_to_bits(
      decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
      &val_dec_2, enter_val_2);

  ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
}
END_TEST

START_TEST(is_less_hard3) {
  ///////////////////////////////
  char enter_val_1[33] = "1.234567891011";
  char enter_val_2[33] = "2.12345678910";
  int expectation = TRUE;

  char res_1[33];
  char res_2[33];
  ///// string to decimal
  s21_decimal val_dec_1 = {0};
  s21_decimal val_dec_2 = {0};
  binary_to_bits(
      decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
      &val_dec_1, enter_val_1);
  binary_to_bits(
      decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
      &val_dec_2, enter_val_2);

  ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
}
END_TEST

START_TEST(is_less_hard4) {
  ///////////////////////////////
  char enter_val_1[33] = "1.234567891011";
  char enter_val_2[33] = "1.12345678910";
  int expectation = FALSE;

  char res_1[33];
  char res_2[33];
  ///// string to decimal
  s21_decimal val_dec_1 = {0};
  s21_decimal val_dec_2 = {0};
  binary_to_bits(
      decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
      &val_dec_1, enter_val_1);
  binary_to_bits(
      decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
      &val_dec_2, enter_val_2);

  ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
}
END_TEST

// START_TEST(is_less_test1) {
//   ///////////////////////////////
//   printf("\nTest 1\n");
//   char enter_val_1[33] = "-12345.67890123";
//   char enter_val_2[33] = "-12346.789123456789";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   // ///// check
//   // printf("\nCheck original bits, value_1:\n");
//   // for (size_t i = 0; i < 4; i++) {
//   //   printf("\nbits[%zu]: ", i);
//   //   print_bits(val_dec_1.bits[i]);
//   // }
//   // printf("\n");

//   // printf("\nCheck original bits, value_2:\n");
//   // for (size_t i = 0; i < 4; i++) {
//   //   printf("\nbits[%zu]: ", i);
//   //   print_bits(val_dec_2.bits[i]);
//   // }
//   // printf("\n");
//   // /////

//   // ///// decimal to string
//   // char decimal_return_1[33];
//   // char decimal_return_2[33];
//   // binary_to_decimal_string(bits_to_binary(val_dec_1), decimal_return_1,
//   //                          &val_dec_1);
//   // binary_to_decimal_string(bits_to_binary(val_dec_2), decimal_return_2,
//   //                          &val_dec_2);

//   // ///// check
//   // printf("\nCheck decimal result, value 1: %s\n", decimal_return_1);
//   // printf("\nCheck decimal result, value 1: %s\n", decimal_return_2);
//   // /////

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test2) {
//   ///////////////////////////////
//   printf("\nTest 2\n");
//   char enter_val_1[33] = "12345";
//   char enter_val_2[33] = "12346";
//   int expectation = TRUE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test3) {
//   ///////////////////////////////
//   printf("\nTest 4\n");
//   char enter_val_1[33] = "12346";
//   char enter_val_2[33] = "12345";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test4) {
//   ///////////////////////////////
//   printf("\nTest 4\n");
//   char enter_val_1[33] = "12345";
//   char enter_val_2[33] = "12345";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test5) {
//   ///////////////////////////////
//   printf("\nTest 5\n");
//   char enter_val_1[33] = "-12345";
//   char enter_val_2[33] = "12346";
//   int expectation = TRUE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test6) {
//   ///////////////////////////////
//   printf("\nTest 6\n");
//   char enter_val_1[33] = "-12345";
//   char enter_val_2[33] = "-12346";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test7) {
//   ///////////////////////////////
//   printf("\nTest 7\n");
//   char enter_val_1[33] = "12345.1";
//   char enter_val_2[33] = "12345";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test8) {
//   ///////////////////////////////
//   printf("\nTest 8\n");
//   char enter_val_1[33] = "12345";
//   char enter_val_2[33] = "12345.1";
//   int expectation = TRUE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test9) {
//   ///////////////////////////////
//   printf("\nTest 9\n");
//   char enter_val_1[33] = "12345.0";
//   char enter_val_2[33] = "12345.1";
//   int expectation = TRUE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test10) {
//   ///////////////////////////////
//   printf("\nTest 10\n");
//   char enter_val_1[33] = "-12345.0";
//   char enter_val_2[33] = "12345.1";
//   int expectation = TRUE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test11) {
//   ///////////////////////////////
//   printf("\nTest 11\n");
//   char enter_val_1[33] = "-12345.0";
//   char enter_val_2[33] = "-12345.1";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test12) {
//   ///////////////////////////////
//   printf("\nTest 12\n");
//   char enter_val_1[33] = "12345.0";
//   char enter_val_2[33] = "-12345.1";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test13) {
//   ///////////////////////////////
//   printf("\nTest 13\n");
//   char enter_val_1[33] = "0";
//   char enter_val_2[33] = "0";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test14) {
//   ///////////////////////////////
//   printf("\nTest 14\n");
//   char enter_val_1[33] = "-0";
//   char enter_val_2[33] = "0";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test15) {
//   ///////////////////////////////
//   printf("\nTest 15\n");
//   char enter_val_1[33] = "-1";
//   char enter_val_2[33] = "0";
//   int expectation = TRUE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test16) {
//   ///////////////////////////////
//   printf("\nTest 16\n");
//   char enter_val_1[33] = "1";
//   char enter_val_2[33] = "0";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test17) {
//   ///////////////////////////////
//   printf("\nTest 17\n");
//   char enter_val_1[33] = "123456789123456789";
//   char enter_val_2[33] = "123456789123456789";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   // printf("s21 result: %d\n", s21_is_less(val_dec_1, val_dec_2));
//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test18) {
//   ///////////////////////////////
//   printf("\nTest 18\n");
//   char enter_val_1[33] = "123456789123456789.123456789";
//   char enter_val_2[33] = "123456789123456789.123456789";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test19) {
//   ///////////////////////////////
//   printf("\nTest 19\n");
//   char enter_val_1[33] = "-123456789123456789.123456789";
//   char enter_val_2[33] = "123456789123456789.123456789";
//   int expectation = TRUE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);
//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test20) {
//   ///////////////////////////////
//   printf("\nTest 20\n");
//   char enter_val_1[33] = "123456789123456789.123456789";
//   char enter_val_2[33] = "-123456789123456789.123456789";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);

//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test21) {
//   ///////////////////////////////
//   printf("\nTest 21\n");
//   char enter_val_1[33] = "-79228162514264337593543950335";
//   char enter_val_2[33] = "79228162514264337593543950335";
//   int expectation = TRUE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);
//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test22) {
//   ///////////////////////////////
//   printf("\nTest 22\n");
//   char enter_val_1[33] = "79228162514264337593543950335";
//   char enter_val_2[33] = "-79228162514264337593543950335";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);
//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test23) {
//   ///////////////////////////////
//   printf("\nTest 23\n");
//   char enter_val_1[33] = "0.9228162514264337593543950335";
//   char enter_val_2[33] = "-0.9228162514264337593543950335";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);
//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test24) {
//   ///////////////////////////////
//   printf("\nTest 24\n");
//   char enter_val_1[33] = "-0.9228162514264337593543950335";
//   char enter_val_2[33] = "0.9228162514264337593543950335";
//   int expectation = TRUE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);
//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

// START_TEST(is_less_test25) {
//   ///////////////////////////////
//   printf("\nTest 25\n");
//   char enter_val_1[33] = "-0.9228162514264337593543950335";
//   char enter_val_2[33] = "-0.9228162514264337593543950335";
//   int expectation = FALSE;

//   char res_1[33];
//   char res_2[33];
//   ///// string to decimal
//   s21_decimal val_dec_1 = {0};
//   s21_decimal val_dec_2 = {0};
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_1, res_1)),
//       &val_dec_1, enter_val_1);
//   binary_to_bits(
//       decimal_string_to_binary(input_to_decimal_strig(enter_val_2, res_2)),
//       &val_dec_2, enter_val_2);
//   ck_assert_int_eq(expectation, s21_is_less(val_dec_1, val_dec_2));
// }
// END_TEST

Suite *test_is_less(void) {
  Suite *s = suite_create("\033[45m S21_IS_LESS \033[0m");
  TCase *tc_is_less = tcase_create("tc_is_less");

  tcase_add_test(tc_is_less, is_less_test1);
  tcase_add_test(tc_is_less, is_less_test2);
  tcase_add_test(tc_is_less, is_less_test3);
  tcase_add_test(tc_is_less, is_less_test4);
  tcase_add_test(tc_is_less, is_less_test5);
  tcase_add_test(tc_is_less, is_less_test6);
  tcase_add_test(tc_is_less, is_less_test7);
  tcase_add_test(tc_is_less, is_less_test8);
  tcase_add_test(tc_is_less, is_less_test9);
  tcase_add_test(tc_is_less, is_less_test10);
  tcase_add_test(tc_is_less, is_less_test11);
  tcase_add_test(tc_is_less, is_less_test12);
  tcase_add_test(tc_is_less, is_less_test13);
  // tcase_add_test(tc_is_less, is_less_test14);
  // tcase_add_test(tc_is_less, is_less_test15);
  // tcase_add_test(tc_is_less, is_less_test16);
  // tcase_add_test(tc_is_less, is_less_test17);
  // tcase_add_test(tc_is_less, is_less_test18);
  // tcase_add_test(tc_is_less, is_less_test19);
  // tcase_add_test(tc_is_less, is_less_test20);
  // tcase_add_test(tc_is_less, is_less_test21);
  // tcase_add_test(tc_is_less, is_less_test22);
  // tcase_add_test(tc_is_less, is_less_test23);
  // tcase_add_test(tc_is_less, is_less_test24);
  // tcase_add_test(tc_is_less, is_less_test25);
  // tcase_add_test(tc_is_less, is_less_test26);
  // tcase_add_test(tc_is_less, is_less_test27);
  // tcase_add_test(tc_is_less, is_less_test28);
  // tcase_add_test(tc_is_less, is_less_test29);
  // tcase_add_test(tc_is_less, is_less_test30);
  // tcase_add_test(tc_is_less, is_less_test31);
  // tcase_add_test(tc_is_less, is_less_test32);
  // tcase_add_test(tc_is_less, is_less_test33);
  // tcase_add_test(tc_is_less, is_less_test34);
  // tcase_add_test(tc_is_less, is_less_test35);
  // tcase_add_test(tc_is_less, is_less_test36);

  tcase_add_test(tc_is_less, is_less_neg_test1);
  tcase_add_test(tc_is_less, is_less_neg_test2);
  tcase_add_test(tc_is_less, is_less_neg_test3);
  tcase_add_test(tc_is_less, is_less_neg_test4);
  tcase_add_test(tc_is_less, is_less_neg_test5);
  tcase_add_test(tc_is_less, is_less_neg_test6);
  tcase_add_test(tc_is_less, is_less_neg_test7);
  tcase_add_test(tc_is_less, is_less_neg_test8);
  tcase_add_test(tc_is_less, is_less_neg_test9);
  tcase_add_test(tc_is_less, is_less_neg_test10);
  tcase_add_test(tc_is_less, is_less_neg_test11);
  tcase_add_test(tc_is_less, is_less_neg_test12);
  tcase_add_test(tc_is_less, is_less_neg_test13);

  tcase_add_test(tc_is_less, test_1);

  tcase_add_test(tc_is_less, test_2);

  tcase_add_test(tc_is_less, test_3);

  tcase_add_test(tc_is_less, test_4);

  tcase_add_test(tc_is_less, test_5);

  tcase_add_test(tc_is_less, test_6);

  tcase_add_test(tc_is_less, test_7);

  // ? hard test
  tcase_add_test(tc_is_less, is_less_hard1);
  tcase_add_test(tc_is_less, is_less_hard2);
  tcase_add_test(tc_is_less, is_less_hard3);
  tcase_add_test(tc_is_less, is_less_hard4);

  suite_add_tcase(s, tc_is_less);

  return s;
}