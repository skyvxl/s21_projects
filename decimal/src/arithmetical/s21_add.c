#include "../s21_decimal.h"
#include "../utils/s21_utils.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int flag = NORMAL;
  clean_decimal(result);

  normalize_decimals(&value_1, &value_2);
  set_decimal_scale(result, get_scale(value_1));

  int sign_1 = get_decimal_sign(value_1);
  int sign_2 = get_decimal_sign(value_2);

  // Одинаковые знаки => обычное сложение
  if (sign_1 == sign_2) {
    unsigned long long sum = 0;
    unsigned int remainder = 0;

    for (int i = 0; i < 3; i++) {
      sum = (unsigned long long)value_1.bits[i] +
            (unsigned long long)value_2.bits[i] + remainder;
      result->bits[i] = (unsigned int)(sum & UINT_MAX);
      remainder = (unsigned int)(sum >> 32);
    }

    set_decimal_sign(result, sign_1);

    // Если остался перенос за пределы 96 бит, считаем что переполнение
    if (remainder) {
      flag = OVERFLOW;
      clean_decimal(result);
    }
  } else {
    // Разные знаки => a + b = a - (-b)
    // Применяем s21_sub
    s21_decimal tmp_value_2 = value_2;
    set_decimal_sign(&tmp_value_2, !sign_2);
    int sub_result = s21_sub(value_1, tmp_value_2, result);

    if (sub_result == OVERFLOW) {
      flag = OVERFLOW;
      clean_decimal(result);
    } else if (sub_result == UNDERFLOW) {
      flag = UNDERFLOW;
      clean_decimal(result);
    }
  }
  return flag;
}