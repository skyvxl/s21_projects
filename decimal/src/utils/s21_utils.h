#ifndef __S21_UTILS_H__
#define __S21_UTILS_H__

#include "../s21_decimal.h"

void print_decimal(s21_decimal *dst);
int is_negative(s21_decimal decimal);

// * comparison

int is_bit3_equal(int value_1, int value_2);
int is_bit3_less(int value_1, int value_2);
int is_bit2_equal(int value_1, int value_2);
int is_bit2_less(int value_1, int value_2);
int is_bit1_equal(s21_decimal value_1, s21_decimal value_2);
int is_bit1_less(s21_decimal value_1, s21_decimal value_2);

// * getters

int get_first_byte(int value);
int get_second_byte(int value);
int get_third_byte(int value);
int get_fourth_byte(int value);
int get_decimal_sign(s21_decimal value);
unsigned int get_scale(s21_decimal value);
int get_decimal_bit(s21_decimal value, int index);
int get_bit(unsigned int value, int index);

// * setters

void set_decimal_scale(s21_decimal *value, int scale);
void set_decimal_sign(s21_decimal *value, int sign);
void set_decimal_bit(s21_decimal *decimal, int index, int value);
void set_bit(unsigned int *word, int index, int value);

// * correct check

int is_decimal_correct(s21_decimal value);
int is_first_byte_correct(int first_byte);
int is_second_byte_correct(int second_byte);
int is_third_byte_correct(int third_byte);
int is_fourth_byte_correct(int fourth_byte);

// ! красивое отображение decimal

char *s21_from_decimal_to_string(s21_decimal value);
s21_decimal s21_from_string_to_decimal(char *string);

// обработка decimal

void delete_leading_zeros(s21_decimal *value);
void normalize_decimals(s21_decimal *value_1, s21_decimal *value_2);
void clean_decimal(s21_decimal *dec);
int multiply_by_int(s21_decimal *value, int multiplier);
void abs_decimal(s21_decimal value, s21_decimal *result);
int is_zero(s21_decimal value);
int compare_abs_3words(s21_decimal val1, s21_decimal val2);
int shift_left(s21_decimal *value, int shift);
int count_scale(float value);
int divide_by_10(s21_decimal *value);
unsigned int divide_by_int(s21_decimal *dec, int divisor);
int is_divisible_by_10(const s21_decimal dec);
void integer_division(s21_decimal dividend, s21_decimal divisor,
                      s21_decimal *quotient, s21_decimal *remainder);

// для дебага деления

int first_bit_of_decimal(s21_decimal value);

// by_toshikoa
//// STRING TO DECIMAL
char *input_to_decimal_strig(char *enter_str, char *res);
int divide_by_two(char *number, int length);
int is_zero_new(char *number, int length);
char *decimal_string_to_binary(char *decimal);
int is_negative_new(char *enter_str);
int scale(char *enter_str);
int binary_string_to_int(const char *binaryString);
void binary_to_bits(char *binary, s21_decimal *dec, char *enter_str);
//// DECIMAL TO STRING
void powerOfTwo(int N, char *result);

//// normalization for is_less
void multiply_by_ten(s21_decimal *value);
void multiply_by_ten_n_times(s21_decimal *value, int n);
void normalize_scales(s21_decimal *value_1, s21_decimal *value_2);

#endif  // __S21_UTILS_H__
