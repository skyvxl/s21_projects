#include "../s21_decimal.h"
#include "../utils/s21_utils.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  if (result == NULL) return CONVERSION_ERROR;

  int scale = get_scale(value);
  // Если число уже целое, возвращаем его
  if (scale == 0) {
    *result = value;
    return NORMAL;
  }

  // Вычисляем целую часть: trunc = truncate(value)
  s21_decimal trunc;
  int status = s21_truncate(value, &trunc);
  if (status != NORMAL) return status;

  // Вычисляем дробную часть: frac = value - trunc
  s21_decimal frac;
  status = s21_sub(value, trunc, &frac);
  if (status != NORMAL) return status;

  // Берём значение дробной части
  s21_decimal abs_frac;
  abs_decimal(frac, &abs_frac);

  // Формируем пороговое значение threshold = 5 * 10^(scale-1)
  s21_decimal threshold;
  clean_decimal(&threshold);
  threshold.bits[0] = 5;  // Изначально число 5
  for (int i = 1; i < scale; i++) {
    status = multiply_by_int(&threshold, 10);
    if (status != NORMAL) return status;
  }
  set_decimal_scale(&threshold,
                    scale);  // Теперь threshold = 5 * 10^(scale-1) с масштабом
                             // scale (т.е. значение 0.5)

  // Если дробная часть >= threshold, округляем от нуля:
  if (s21_is_greater_or_equal(abs_frac, threshold)) {
    s21_decimal one;
    status = s21_from_int_to_decimal(1, &one);  // число 1 с масштабом 0
    if (status != NORMAL) return status;
    if (get_decimal_sign(value) == 0) {
      // Для положительного числа: прибавляем 1
      status = s21_add(trunc, one, &trunc);
    } else {
      // Для отрицательного числа: вычитаем 1
      status = s21_sub(trunc, one, &trunc);
    }
    if (status != NORMAL) return status;
  }

  // Устанавливаем масштаб результата равным 0 (целое число)
  set_decimal_scale(&trunc, 0);
  *result = trunc;
  return NORMAL;
}
