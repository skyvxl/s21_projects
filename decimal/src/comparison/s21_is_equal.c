#include "../s21_decimal.h"
#include "../utils/s21_utils.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  delete_leading_zeros(&value_1);
  delete_leading_zeros(&value_2);

  // сравниваем преобразованные децимал
  int result = (is_bit3_equal(value_1.bits[2], value_2.bits[2]) &&
                is_bit3_equal(value_1.bits[1], value_2.bits[1]) &&
                is_bit3_equal(value_1.bits[0], value_2.bits[0]) &&
                is_bit3_equal(value_1.bits[3], value_2.bits[3])) ||
               (is_zero(value_1) && is_zero(value_2));
  return result;
}