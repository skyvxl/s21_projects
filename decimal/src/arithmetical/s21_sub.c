#include "../s21_decimal.h"
#include "../utils/s21_utils.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int flag = NORMAL;
  clean_decimal(result);

  normalize_decimals(&value_1, &value_2);
  set_decimal_scale(result, get_scale(value_1));

  int sign_1 = get_decimal_sign(value_1);
  int sign_2 = get_decimal_sign(value_2);

  if (sign_1 == sign_2) {
    // Одинаковые знаки -> "чистое" вычитание по модулю
    // Сравниваем модули, чтобы определить, кто больше
    s21_decimal abs1 = value_1, abs2 = value_2;
    set_decimal_sign(&abs1, 0);
    set_decimal_sign(&abs2, 0);

    int cmp = compare_abs_3words(abs1, abs2);
    if (cmp == 0) {
      // Модули равны => результат = 0
      clean_decimal(result);
    } else {
      s21_decimal bigger = abs1, smaller = abs2;
      int final_sign = sign_1;  // результат по умолчанию имеет знак a

      if (cmp < 0) {
        // |value_2| > |value_1| => придётся вычитать |value_1| из |value_2|
        bigger = abs2;
        smaller = abs1;
        // Меняем знак результата на противоположный
        final_sign = !sign_1;
      }

      // Вычитаем smaller из bigger (побитово, как раньше)
      unsigned long long res = 0ULL;
      int borrow = 0;

      for (int i = 0; i < 3; i++) {
        res = (unsigned long long)bigger.bits[i] - smaller.bits[i] - borrow;
        borrow = (int)((res >> 32) & 1);
        result->bits[i] = (unsigned int)(res & UINT_MAX);
      }
      // Для упрощённого кода предполагаем, что (bigger >= smaller) => borrow=0

      // Устанавливаем итоговый знак
      set_decimal_sign(result, final_sign);
    }
  } else {
    // Разные знаки => (a - b) = a + (-b)
    s21_decimal tmp_value_2 = value_2;
    set_decimal_sign(&tmp_value_2, !sign_2);  // меняем знак второго
    int add_result = s21_add(value_1, tmp_value_2, result);

    if (add_result == OVERFLOW) {
      flag = OVERFLOW;
      clean_decimal(result);
    } else if (add_result == UNDERFLOW) {
      flag = UNDERFLOW;
      clean_decimal(result);
    } else {
      // При желании можно явно проставить знак, но s21_add и так сделает
      // правильно
      set_decimal_sign(result, get_decimal_sign(value_1));
    }
  }
  return flag;
}
