#include "../s21_decimal.h"
#include "../utils/s21_utils.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int status = CONVERSION_OK;
  clean_decimal(dst);
  if (isnan(src) || isinf(src)) {
    status = CONVERSION_ERROR;
  } else {
    int sign = 0;
    if (src < 0) {
      sign = 1;
      src = -src;
    }
    // Определяем scale с помощью функции count_scale (например, для 1.234567f
    // может получиться 7)
    int scale = count_scale(src);
    // Для повышения точности используем double для промежуточных вычислений
    double dtemp = src;
    for (int i = 0; i < scale; i++) {
      dtemp *= 10.0;
    }
    // Округляем полученное число до целого
    unsigned long long int_part = (unsigned long long)round(dtemp);
    // Если результат делится на 10, возможно scale переизбыточен.
    // Уменьшаем scale, пока число делится на 10 и scale > 0.
    while (scale > 0 && int_part % 10 == 0) {
      int_part /= 10;
      scale--;
    }
    dst->bits[0] = (unsigned int)(int_part & 0xFFFFFFFFULL);
    dst->bits[1] = (unsigned int)((int_part >> 32) & 0xFFFFFFFFULL);
    dst->bits[2] = 0;
    set_decimal_scale(dst, scale);
    set_decimal_sign(dst, sign);
  }
  return status;
}
