#include "../s21_decimal.h"
#include "../utils/s21_utils.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int value_1_sign = get_decimal_sign(value_1);
  int value_2_sign = get_decimal_sign(value_2);

  if (s21_is_equal(value_1, value_2)) return FALSE;

  int result = TRUE;
  // сравнение по знаку
  if (value_1_sign > value_2_sign)
    result = TRUE;
  else if (value_1_sign < value_2_sign)
    result = FALSE;

  // если оба знака -
  else if (value_1_sign == 1) {
    set_decimal_sign(&value_1, 0);
    set_decimal_sign(&value_2, 0);
    result = !s21_is_less(value_1, value_2);

    // если оба знака +
    // не было нормализации, и разное кол-во знаков после запятой
    // давало неверный результат

  } else {
    int scale_1 = get_scale(value_1);
    int scale_2 = get_scale(value_2);

    if (scale_1 != scale_2) {
      normalize_scales(&value_1, &value_2);
    }

    if (value_1.bits[2] != value_2.bits[2])
      result = value_1.bits[2] < value_2.bits[2];
    else if (value_1.bits[1] != value_2.bits[1])
      result = value_1.bits[1] < value_2.bits[1];
    else
      result = value_1.bits[0] < value_2.bits[0];
  }
  return result;
}