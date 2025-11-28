#include "../s21_decimal.h"
#include "../utils/s21_utils.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int status = CALCULATE_OK;

  if (!result || !is_decimal_correct(value)) {
    status = CALCULATE_ERROR;
  } else {
    clean_decimal(result);
    int sign = get_decimal_sign(value);

    if (get_scale(value) == 0) {
      memcpy(result, &value, sizeof(s21_decimal));
    } else {
      int scale = get_scale(value);
      set_decimal_scale(result, 0);
      set_decimal_scale(&value, 0);
      s21_decimal ten = {{10, 0, 0, 0}};
      for (int i = 1; i < scale; i++) multiply_by_int(&ten, 10);
      s21_decimal remainder;
      integer_division(value, ten, result, &remainder);
      set_decimal_sign(result, sign);
    }
  }

  return status;
}