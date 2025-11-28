#include "../s21_decimal.h"
#include "../utils/s21_utils.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  // Проверка входных данных
  if (!result) return CONVERSION_ERROR;
  if (!is_decimal_correct(value_1) || !is_decimal_correct(value_2)) {
    clean_decimal(result);
    return CONVERSION_ERROR;
  }
  if (is_zero(value_2)) {  // деление на 0
    clean_decimal(result);
    return DIVISION_BY_ZERO;
  }
  clean_decimal(result);

  // Сохраняем знак результата (XOR знаков операндов)
  int sign1 = get_decimal_sign(value_1);
  int sign2 = get_decimal_sign(value_2);
  int result_sign = sign1 ^ sign2;

  // Получаем масштабы операндов
  unsigned int scale_a = get_scale(value_1);
  unsigned int scale_b = get_scale(value_2);
  int diff = (int)scale_a - (int)scale_b;

  // Берём абсолютные значения и сбрасываем масштабы – работаем с целыми
  // 96‑битными числами.
  s21_decimal A, B;
  abs_decimal(value_1, &A);
  abs_decimal(value_2, &B);
  set_decimal_scale(&A, 0);
  set_decimal_scale(&B, 0);

  /* Если у делимого меньше масштаба (diff < 0), домножаем A на 10^(–diff).
     Это приводит к приведению операндов к общему целому представлению. */
  if (diff < 0) {
    int times = -diff;
    for (int i = 0; i < times; i++) {
      if (multiply_by_int(&A, 10) != NORMAL) {
        clean_decimal(result);
        return UNDERFLOW;  // в данном случае возвращаем UNDERFLOW
      }
    }
    diff = 0;
  }
  // Изначальный масштаб результата равен скорректированному diff.
  int result_scale = diff;

  // Выполняем целочисленное деление: получаем частное Q и остаток R.
  s21_decimal Q, R;
  clean_decimal(&Q);
  clean_decimal(&R);
  integer_division(A, B, &Q, &R);

  /* Если остаток не равен 0 и масштаб меньше 28, добиваем точность:
     в каждой итерации умножаем остаток на 10, вычисляем очередную цифру
     и «приписываем» её к Q, одновременно увеличивая масштаб. */
  while (!is_zero(R) && result_scale < 28) {
    if (multiply_by_int(&R, 10) != NORMAL) break;
    s21_decimal digit, new_R;
    clean_decimal(&digit);
    clean_decimal(&new_R);
    integer_division(R, B, &digit, &new_R);
    if (multiply_by_int(&Q, 10) != NORMAL) break;
    s21_decimal tmp;
    if (s21_add(Q, digit, &tmp) != NORMAL) break;
    Q = tmp;
    R = new_R;
    result_scale++;
  }

  // Если деление закончилось точно (остаток равен 0), попробуем убрать
  // незначащие нули:
  while (result_scale > 0 && is_divisible_by_10(Q)) {
    divide_by_int(&Q, 10);
    result_scale--;
  }

  // Если итоговый масштаб выходит за пределы [0, 28], вернуть ошибку.
  if (result_scale < 0 || result_scale > 28) {
    clean_decimal(result);
    return CALCULATE_ERROR;
  }

  // Устанавливаем результат, его масштаб и знак.
  *result = Q;
  set_decimal_scale(result, result_scale);
  set_decimal_sign(result, result_sign);

  return NORMAL;
}
