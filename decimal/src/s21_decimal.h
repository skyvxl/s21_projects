#ifndef __S21_DECIMALS_H__
#define __S21_DECIMALS_H__

#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __MACH__
#undef OVERFLOW
#undef UNDERFLOW
#endif

/// ====== STRUCTURES ====== ///

typedef struct {
  unsigned int bits[4];
} s21_decimal;

/// ====== ENUMS ====== ///
// коды возвращаемые функциями arithmetical operation
enum { NORMAL, OVERFLOW, UNDERFLOW, DIVISION_BY_ZERO, ERROR };

// Коды возвращаемые функциями сравнения
enum {
  FALSE,
  TRUE,
};

// Коды возвращаемые функциями conversion operation
enum { CONVERSION_OK, CONVERSION_ERROR };

// Коды возвращаемые функциями other operation
enum { CALCULATE_OK, CALCULATE_ERROR };

/// ====== FUNCTIONS ====== ///
// ?part 1 arithmetical operation

int s21_add(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // * у меня есть вопросы, но в целом окей
int s21_sub(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // * допустим
int s21_mul(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // * тесты прошли, значит работает
int s21_div(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // *

// *part 2 comparison operation
int s21_is_less(s21_decimal value_1, s21_decimal value_2);              // *
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal);             // *
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);           // *
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);  // *
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);             // *
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);         // *

// *part 3 conversion operation

int s21_from_int_to_decimal(int src, s21_decimal *dst);      // *
int s21_from_float_to_decimal(float src, s21_decimal *dst);  // *
int s21_from_decimal_to_int(s21_decimal src, int *dst);      // *
int s21_from_decimal_to_float(s21_decimal src, float *dst);  // *

// *part 4 other functions

int s21_floor(s21_decimal value, s21_decimal *result);  // *
int s21_round(s21_decimal value, s21_decimal *result);  // *
int s21_truncate(s21_decimal value,
                 s21_decimal *result);                   // *
int s21_negate(s21_decimal value, s21_decimal *result);  // *

#endif  // __S21_DECIMALS_H__