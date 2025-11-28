#include "../s21_decimal.h"
#include "../utils/s21_utils.h"

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
  if (!dst) return CONVERSION_ERROR;

  clean_decimal(dst);
  if (src < 0 && src != INT_MIN) {
    set_decimal_sign(dst, 1);
    dst->bits[0] = (unsigned int)(-src);
  } else if (src < 0) {
    dst->bits[0] = (unsigned int)src;
    set_decimal_sign(dst, 1);
  } else {
    dst->bits[0] = (unsigned int)src;
  }
  return CONVERSION_OK;
}