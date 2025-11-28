#include "../s21_decimal.h"
#include "../utils/s21_utils.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) return ERROR;

  delete_leading_zeros(&value_1);
  delete_leading_zeros(&value_2);

  if ((!is_decimal_correct(value_1)) || (!is_decimal_correct(value_2)))
    return UNDERFLOW;

  int status = NORMAL;
  s21_decimal abs_value_1, abs_value_2;
  abs_decimal(value_1, &abs_value_1);
  abs_decimal(value_2, &abs_value_2);

  // Инициализация промежуточного результата
  s21_decimal temp_result = {{0, 0, 0, 0}};

  // Умножение
  for (int i = 0; i < 96; i++) {
    if (get_decimal_bit(abs_value_2, i)) {
      s21_decimal shifted_value_1 = abs_value_1;
      if (shift_left(&shifted_value_1, i) == OVERFLOW) {
        status = OVERFLOW;
      };
      if (s21_add(temp_result, shifted_value_1, &temp_result) == OVERFLOW) {
        status = OVERFLOW;
      };
    }
  }

  // Установка результата
  *result = temp_result;

  // Установка масштаба результата
  int scale_1 = get_scale(value_1);
  int scale_2 = get_scale(value_2);
  int result_scale = scale_1 + scale_2;
  set_decimal_scale(result, result_scale);
  delete_leading_zeros(result);

  if (get_scale(*result) > 28) {
    status = UNDERFLOW;  // Переполнение масштаба
  }

  // Установка знака результата
  int sign_1 = get_decimal_sign(value_1);
  int sign_2 = get_decimal_sign(value_2);
  if (sign_1 != sign_2) set_decimal_sign(result, 1);

  if (status != NORMAL) clean_decimal(result);

  return status;
}