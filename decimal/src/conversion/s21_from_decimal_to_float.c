#include <float.h>

#include "../s21_decimal.h"
#include "../utils/s21_utils.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (!dst || !is_decimal_correct(src)) {
    return CONVERSION_ERROR;
  }
  int status = CONVERSION_OK;
  int sign = get_decimal_sign(src);
  unsigned int scale = get_scale(src);

  float integer = (float)src.bits[0];
  integer += (float)src.bits[1] * powf(2, 32);
  integer += (float)src.bits[2] * powf(2, 64);

  float divisor = powf(10.0f, (float)scale);

  float value = integer / divisor;

  if (sign) {
    value = -value;
  }

  if (value > FLT_MAX || value < -FLT_MAX) {
    status = CONVERSION_ERROR;
  } else
    *dst = value;
  return status;
}