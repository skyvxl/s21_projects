#include "../s21_decimal.h"
#include "../utils/s21_utils.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int flag = CONVERSION_OK;

  // Трuncate decimal part
  s21_decimal decimal_part;
  s21_truncate(src, &decimal_part);

  // Проверка переполнения
  if (decimal_part.bits[2] != 0 || decimal_part.bits[1] != 0 ||
      decimal_part.bits[0] > (unsigned)INT_MIN) {
    flag = CONVERSION_ERROR;
  }
  // отдельная проверка на INT_MIN
  if (!is_negative(decimal_part) && decimal_part.bits[0] == (unsigned)INT_MIN) {
    flag = CONVERSION_ERROR;
  }

  if (flag == CONVERSION_OK) {
    *dst = (int)decimal_part.bits[0];
    if (is_negative(decimal_part) && decimal_part.bits[0] != (unsigned)INT_MIN)
      *dst = -*dst;
  }

  return flag;
}