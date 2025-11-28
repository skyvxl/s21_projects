#include "../s21_decimal.h"
#include "../utils/s21_utils.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int status = CALCULATE_OK;

  if (!result || !is_decimal_correct(value)) {
    return 1;
  }

  clean_decimal(result);
  int sign = get_decimal_sign(value);
  s21_decimal truncated;
  clean_decimal(&truncated);
  s21_truncate(value, &truncated);
  *result = truncated;

  // Для отрицательных чисел, если число не целое, вычитаем 1
  if (sign && !s21_is_equal(value, truncated)) {
    s21_decimal one;
    clean_decimal(&one);
    one.bits[0] = 1;
    s21_sub(truncated, one, result);
  }

  return status;
}