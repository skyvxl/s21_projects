#include "s21_utils.h"

// обработка decimal

void delete_leading_zeros(s21_decimal* value) {
  int scale = get_scale(*value);

  while (scale > 0 && is_divisible_by_10(*value)) {
    divide_by_int(value, 10);
    scale--;
  }
  set_decimal_scale(value, scale);
}

int shift_left(s21_decimal* value, int shift) {
  for (int s = 0; s < shift; s++) {
    unsigned int overflow = 0;
    for (int i = 0; i < 3; i++) {
      // сохраняем самый левый бит(31 - й бит)
      unsigned int new_overflow = value->bits[i] >> 31;
      // добавляем бит переполнения (overflow) из предыдущего блока
      value->bits[i] = (value->bits[i] << 1) | overflow;
      overflow = new_overflow;
    }
    // Проверка переполнения самого старшего разряда
    if (overflow) {
      // Если переполнение произошло, возвращаем ошибку
      return OVERFLOW;
    }
  }
  return NORMAL;
}

// Умножение s21_decimal на целое число
int multiply_by_int(s21_decimal* value, int multiplier) {
  unsigned long long temp_result[4] = {0};

  // Умножение каждого 32-битного блока
  for (int i = 0; i < 3; i++) {
    // 8 byte
    unsigned long long mul = (unsigned long long)value->bits[i] * multiplier;
    temp_result[i] += mul;
    // Переносим переполнение в следующий блок
    if (temp_result[i] > 0xFFFFFFFF) {
      temp_result[i + 1] += temp_result[i] >> 32;
      temp_result[i] &= 0xFFFFFFFF;
    }
  }

  // Проверка на переполнение
  if (temp_result[3]) {
    return OVERFLOW;
  }

  // Записываем результат обратно в s21_decimal
  value->bits[0] = (unsigned int)temp_result[0];
  value->bits[1] = (unsigned int)temp_result[1];
  value->bits[2] = (unsigned int)temp_result[2];

  return NORMAL;
}

void normalize_decimals(s21_decimal* value_1, s21_decimal* value_2) {
  int scale_1 = get_scale(*value_1);
  int scale_2 = get_scale(*value_2);
  int diff;
  if (scale_1 < scale_2) {
    diff = scale_2 - scale_1;
    for (int i = 0; i < diff; i++) {
      if (multiply_by_int(value_1, 10) == OVERFLOW) {
        // Обработка переполнения
        // Можно установить флаг ошибки или вернуть значение ошибки
        return;
      }
    }
    set_decimal_scale(value_1, scale_2);
  } else if (scale_2 < scale_1) {
    diff = scale_1 - scale_2;
    for (int i = 0; i < diff; i++) {
      if (multiply_by_int(value_2, 10) == OVERFLOW) {
        // Обработка переполнения
        // Можно установить флаг ошибки или вернуть значение ошибки
        return;
      }
    }
    set_decimal_scale(value_2, scale_1);
  }
}

void clean_decimal(s21_decimal* dec) {
  for (int i = 0; i < 4; i++) {
    dec->bits[i] = 0;
  }
}

// * getters

int get_second_byte(int value) {
  return value & 0b00000000111111110000000000000000;
}

int get_decimal_sign(s21_decimal value) {
  unsigned int result = 1;
  result <<= 31;
  result &= value.bits[3];
  result >>= 31;
  return result;
}

unsigned int get_scale(s21_decimal value) {
  unsigned int scale = get_second_byte(value.bits[3]);
  scale >>= 16;
  return scale;
}

int get_decimal_bit(s21_decimal value, int index) {
  int bit = -1;

  if (index >= 0 && index <= 31) {
    bit = get_bit(value.bits[0], index);
  } else if (index >= 32 && index <= 63) {
    bit = get_bit(value.bits[1], index % 32);
  } else if (index >= 64 && index <= 95) {
    bit = get_bit(value.bits[2], index % 32);
  }

  return bit;
}

int get_bit(unsigned int value, int index) {
  int result = 0;
  int mask = 1 << index;
  result = (value & mask) >> index;
  return result;
}

// * setters
void set_decimal_sign(s21_decimal* value, int sign) {
  int mask = 0x7FFFFFFF;
  value->bits[3] &= mask;
  value->bits[3] |= (sign << 31);
}

void set_decimal_scale(s21_decimal* value, int scale) {
  scale <<= 16;
  unsigned int mask =
      0xFF00FFFF;  // 0xFF00FFFF == 0b11111111000000001111111111111111
  value->bits[3] &= mask;
  value->bits[3] |= scale;
}

void set_decimal_bit(s21_decimal* decimal, int index, int value) {
  if (index >= 0 && index <= 31) {
    set_bit(&decimal->bits[0], index, value);
  } else if (index >= 32 && index <= 63) {
    set_bit(&decimal->bits[1], index, value);
  } else if (index >= 64 && index <= 95) {
    set_bit(&decimal->bits[2], index, value);
  }
}

void set_bit(unsigned int* word, int index, int value) {
  int mask = 1 << index;
  *word &= ~mask;
  value <<= index;
  *word |= value;
}

// * correct check
int is_decimal_correct(s21_decimal value) {
  int result = 1;
  int sign = (value.bits[3] >> 31) & 1;
  if (sign != 0 && sign != 1) result = 0;
  int scale = (value.bits[3] >> 16) & 0x7F;
  if (scale < 0 || scale > 28) result = 0;
  // Проверяем, что младшие 16 бит и биты 24-30 равны нулю:
  if ((value.bits[3] & 0x0000FFFF) != 0) result = 0;
  if ((value.bits[3] & 0x7F000000) != 0) result = 0;
  return result;
}

// * comparison
int is_bit3_equal(int value_1, int value_2) { return value_1 == value_2; }

int is_negative(s21_decimal decimal) {
  int result = 1 << 31;
  result &= decimal.bits[3];
  result >>= 31;
  return result;
}

void abs_decimal(s21_decimal value, s21_decimal* result) {
  *result = value;
  set_decimal_sign(result, 0);
}

int is_zero(s21_decimal value) {
  int status = NORMAL;
  if ((value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0)) {
    status = DIVISION_BY_ZERO;
  }
  return status;
}

int compare_abs_3words(s21_decimal val1, s21_decimal val2) {
  int result = 0;
  int i = 2;
  int flag = 0;

  while (i >= 0 && flag == 0) {
    if (val1.bits[i] != val2.bits[i]) {
      result = (val1.bits[i] > val2.bits[i]) ? 1 : -1;
      flag = 1;
    }
    i--;
  }
  return result;
}

/** `1e-7f` - Очень маленькое число с плавающей точкой (0.0000001) - компромисс
 * между точностью и скоростью вычислений
 */
int count_scale(float value) {
  int scale = 0;
  float temp = fabsf(value);
  // Ограничиваем масштаб до 28 знаков и проверяем, что дробная часть значима
  while (fabsf(temp - roundf(temp)) > 1e-7f && scale < 28) {
    temp *= 10;
    scale++;
  }
  return scale;
}

unsigned int divide_by_int(s21_decimal* dec, int divisor) {
  unsigned long long rem = 0;
  for (int i = 2; i >= 0; i--) {
    unsigned long long current = (rem << 32) | dec->bits[i];
    dec->bits[i] = (unsigned int)(current / divisor);
    rem = current % divisor;
  }
  return rem;
}

int is_divisible_by_10(const s21_decimal dec) {
  s21_decimal temp = dec;
  unsigned int rem = divide_by_int(&temp, 10);
  return (rem == 0);
}

// Функция целочисленного деления 96-битных чисел.
// dividend = divisor * *quotient + *remainder.
void integer_division(s21_decimal dividend, s21_decimal divisor,
                      s21_decimal* quotient, s21_decimal* remainder) {
  clean_decimal(quotient);
  clean_decimal(remainder);
  for (int i = 95; i >= 0; i--) {
    // Сдвигаем остаток влево на 1 бит.
    shift_left(remainder, 1);
    // Берём i–й бит делимого.
    int bit = get_decimal_bit(dividend, i);
    set_decimal_bit(remainder, 0, bit);
    // Если остаток >= делителя, вычитаем делитель и ставим бит в частном.
    if (compare_abs_3words(*remainder, divisor) >= 0) {
      s21_decimal temp;
      s21_sub(*remainder, divisor, &temp);
      *remainder = temp;
      set_decimal_bit(quotient, i, 1);
    }
  }
}

////
////
////
////
//// by_toshikoa
//// STRING TO DECIMAL
////
////
////
char* input_to_decimal_strig(char* enter_str, char* res) {
  size_t j = 0;
  for (size_t i = 0; enter_str[i] != '\0'; i++) {
    if (enter_str[i] != '-' && enter_str[i] != ',' && enter_str[i] != '.') {
      res[j++] = enter_str[i];
    }
  }
  res[j] = '\0';
  return res;
}

// Функция для деления большого числа на 2
// По сути просто реализация деления "в столбик"
int divide_by_two(char* number, int length) {
  int carry = 0;  // переменная для хранения переноса
  for (int i = 0; i < length; i++) {
    // преобразуем символ цифры в число - используем ASCII
    int digit = number[i] - '0';
    int new_digit = carry * 10 + digit;
    number[i] = (new_digit / 2) + '0';
    carry = new_digit % 2;  // сохраняем остаток от деления для следующего шага
  }
  return carry;
}

// Функция для проверки, равно ли число нулю
int is_zero_new(char* number, int length) {
  for (int i = 0; i < length; i++) {
    if (number[i] != '0') {
      return 0;
    }
  }
  return 1;
}

// Функция для перевода числа в двоичную систему
char* decimal_string_to_binary(char* decimal) {
  static char binary[96 + 1] = {0};  // +1 для нуль-терминатора
  char temp[96 + 1] = {0};  // Временный массив для хранения битов
  int binary_index = 0;
  int length = strlen(decimal);

  // Преобразуем число в двоичное представление
  while (!is_zero_new(decimal, length)) {
    temp[binary_index++] = divide_by_two(decimal, length) + '0';
  }

  // Дополняем временный массив нулями до 96 символов
  while (binary_index < 96) {
    temp[binary_index++] = '0';
  }

  // Переворачиваем результат, так как биты собирались в обратном порядке
  for (int i = 0; i < 96; i++) {
    binary[i] = temp[96 - 1 - i];
  }

  binary[96] = '\0';  // Добавляем нуль-терминатор
  return binary;
}

int is_negative_new(char* enter_str) {
  int sign = 0;
  for (size_t i = 0; enter_str[i] != '\0'; i++) {
    if (enter_str[0] == '-') {
      sign = 1;
    }
  }
  return sign;
}

int scale(char* enter_str) {
  int scale = 0;
  int found_comma = 0;
  for (size_t i = 0; enter_str[i] != '\0'; i++) {
    if ((enter_str[i] == ',' || enter_str[i] == '.') && !found_comma) {
      found_comma = 1;
      continue;
    }
    if (found_comma) {
      scale++;  // Считаем цифры после запятой
    }
  }
  return scale;
}

int binary_string_to_int(const char* binaryString) {
  // преобразует строку в число типа long int
  return (int)strtol(binaryString, NULL, 2);
}

void binary_to_bits(char* binary, s21_decimal* dec, char* enter_str) {
  // void binary_to_bits(char *binary) {
  char bits_0[33] = {0};
  char bits_1[33] = {0};
  char bits_2[33] = {0};
  char bits_3[33] = {0};
  strncpy(bits_2, binary, 32);
  strncpy(bits_1, binary + 32, 32);
  strncpy(bits_0, binary + 64, 32);

  for (size_t i = 0; i < 32; i++) {
    bits_3[i] = '0';
  }

  // scale check
  // printf("\nScale: %d", scale(enter_str));
  int scale_value = scale(enter_str);

  for (int i = 0; i < 8; i++) {
    if (scale_value & (1 << i)) {
      bits_3[15 - i] = '1';
    } else {
      bits_3[15 - i] = '0';
    }
  }

  if (is_negative_new(enter_str)) bits_3[0] = '1';

  // check
  // printf("\nOriginal bits:\n");
  // printf("\nstr_0: %s\n", bits_0);
  // printf("str_1: %s\n", bits_1);
  // printf("str_2: %s\n", bits_2);
  // printf("str_3: %s\n", bits_3);

  dec->bits[0] = binary_string_to_int(bits_0);
  dec->bits[1] = binary_string_to_int(bits_1);
  dec->bits[2] = binary_string_to_int(bits_2);
  dec->bits[3] = binary_string_to_int(bits_3);
}

/////
///// normalization for is_less
/////
void multiply_by_ten(s21_decimal* value) {
  uint64_t temp = (uint64_t)value->bits[0] * 10;
  value->bits[0] = (uint32_t)(temp & 0xFFFFFFFF);
  temp = (uint64_t)value->bits[1] * 10 + (temp >> 32);
  value->bits[1] = (uint32_t)(temp & 0xFFFFFFFF);
  temp = (uint64_t)value->bits[2] * 10 + (temp >> 32);
  value->bits[2] = (uint32_t)(temp & 0xFFFFFFFF);
}

void multiply_by_ten_n_times(s21_decimal* value, int n) {
  for (int i = 0; i < n; i++) {
    multiply_by_ten(value);
  }
}

void normalize_scales(s21_decimal* value_1, s21_decimal* value_2) {
  int scale_1 = get_scale(*value_1);
  int scale_2 = get_scale(*value_2);

  if (scale_1 < scale_2) {
    multiply_by_ten_n_times(value_1, scale_2 - scale_1);
    set_decimal_scale(value_1, scale_2);
  } else if (scale_1 > scale_2) {
    multiply_by_ten_n_times(value_2, scale_1 - scale_2);
    set_decimal_scale(value_2, scale_1);
  }
}