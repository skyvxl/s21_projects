#include "../s21_decimal.h"

// Возвращает результат умножения указанного Decimal на -1
int s21_negate(s21_decimal value, s21_decimal *result) {
  result->bits[0] = value.bits[0];
  result->bits[1] = value.bits[1];
  result->bits[2] = value.bits[2];
  // инвертируем (0->1 или наоборот) последний бит
  // используем операцию побитового исключающего ИЛИ (XOR) - ^=
  result->bits[3] = value.bits[3] ^= (1 << 31);
  // Используем XOR с 1, чтобы инвертировать последний бит

  return 0;
}
