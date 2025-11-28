#include "../decimal_test.h"

void test_round_f(s21_decimal decimal, s21_decimal decimal_check) {
  s21_decimal result;

  int code = s21_round(decimal, &result);

  int sign_check = get_decimal_sign(decimal);

  int sign_result = get_decimal_sign(result);

  ck_assert_int_eq(code, 0);

  ck_assert_int_eq(s21_is_equal(result, decimal_check), 1);

  ck_assert_int_eq(sign_check, sign_result);
}

START_TEST(test_round_1) {
  // 792281625.14264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};

  int code = s21_round(decimal, NULL);

  ck_assert_int_eq(code, 1);
}

START_TEST(test_round_2) {
  // степень 154 (показатель степени должен быть от 0 до 28)

  // биты 0-15 не нули

  // биты 24-30 не нули

  s21_decimal decimal = {{0, 0, 0, 1000000000}};

  s21_decimal result;

  int code = s21_round(decimal, &result);

  ck_assert_int_eq(code, 1);
}

END_TEST

START_TEST(test_round_3) {
  // степень 29 (показатель степени должен быть от 0 до 28)

  s21_decimal decimal = {{-1, 0, 0, 0x1D0000}};

  s21_decimal result;

  int code = s21_round(decimal, &result);

  ck_assert_int_eq(code, 1);
}

END_TEST

START_TEST(test_round_4) {
  // степень 29 (показатель степени должен быть от 0 до 28)

  s21_decimal decimal = {{0, 0, 0, 0x1D0000}};

  s21_decimal result;

  int code = s21_round(decimal, &result);

  ck_assert_int_eq(code, 1);
}

END_TEST

START_TEST(test_round_5) {
  // степень 28 (что корректно), но биты 0-15 не нули (младший бит)

  s21_decimal decimal = {{-1, 0, 0, 0x1C0001}};

  s21_decimal result;

  int code = s21_round(decimal, &result);

  ck_assert_int_eq(code, 1);
}

END_TEST

START_TEST(test_round_6) {
  // степень 28 (что корректно), но биты 0-15 не нули (старший бит)

  s21_decimal decimal = {{-1, 0, 0, 0x1C8000}};

  s21_decimal result;

  int code = s21_round(decimal, &result);

  ck_assert_int_eq(code, 1);
}

END_TEST

START_TEST(test_round_7) {
  // степень 28 (что корректно), но биты 24-30 не нули (младший бит)

  s21_decimal decimal = {{-1, 0, 0, 0x11C0000}};

  s21_decimal result;

  int code = s21_round(decimal, &result);

  ck_assert_int_eq(code, 1);
}

END_TEST

START_TEST(test_round_8) {
  // степень 28 (что корректно), но биты 24-30 не нули (старший бит)

  s21_decimal decimal = {{-1, 0, 0, 0x401C0000}};

  s21_decimal result;

  int code = s21_round(decimal, &result);

  ck_assert_int_eq(code, 1);
}

END_TEST

START_TEST(test_round_9) {
  // Просто все единицы

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};

  s21_decimal result;

  int code = s21_round(decimal, &result);

  ck_assert_int_eq(code, 1);
}

END_TEST

START_TEST(test_round_10) {
  // 79228162514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  // 79228162514264337593543950335

  s21_decimal decimal_check = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_11) {
  // -2640938749860586450661146624.0

  s21_decimal decimal = {{0x0, 0x0, 0x55555555, 0x80010000}};

  // -2640938749860586450661146624

  s21_decimal decimal_check = {{0x0, 0x80000000, 0x8888888, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_12) {
  // -79228162514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  // -79228162514264337593543950335

  s21_decimal decimal_check = {

      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_13) {
  // 7922816251426433759354395033.5

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}};

  // 7922816251426433759354395034

  s21_decimal decimal_check = {{0x9999999A, 0x99999999, 0x19999999, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_14) {
  // -7922816251426433759354395033.5

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}};

  // -7922816251426433759354395034

  s21_decimal decimal_check = {

      {0x9999999A, 0x99999999, 0x19999999, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_15) {
  // 792281625142643375935439503.35

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x20000}};

  // 792281625142643375935439503

  s21_decimal decimal_check = {{0x28F5C28F, 0xF5C28F5C, 0x28F5C28, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_16) {
  // -792281625142643375935439503.35

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80020000}};

  // -792281625142643375935439503

  s21_decimal decimal_check = {{0x28F5C28F, 0xF5C28F5C, 0x28F5C28, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_17) {
  // 79228162514264337593543950.335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x30000}};

  // 79228162514264337593543950

  s21_decimal decimal_check = {{0x9DB22D0E, 0x4BC6A7EF, 0x418937, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_18) {
  // -79228162514264337593543950.335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80030000}};

  // -79228162514264337593543950

  s21_decimal decimal_check = {{0x9DB22D0E, 0x4BC6A7EF, 0x418937, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_19) {
  // 7922816251426433759354395.0335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x40000}};

  // 7922816251426433759354395

  s21_decimal decimal_check = {{0x295E9E1B, 0xBAC710CB, 0x68DB8, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_20) {
  // -7922816251426433759354395.0335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80040000}};

  // -7922816251426433759354395

  s21_decimal decimal_check = {{0x295E9E1B, 0xBAC710CB, 0x68DB8, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_21) {
  // 792281625142643375935439.50335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x50000}};

  // 792281625142643375935440

  s21_decimal decimal_check = {{0x84230FD0, 0xAC471B47, 0xA7C5, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_22) {
  // -792281625142643375935439.50335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80050000}};

  // -792281625142643375935440

  s21_decimal decimal_check = {{0x84230FD0, 0xAC471B47, 0xA7C5, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_23) {
  // 79228162514264337593543.950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x60000}};

  // 79228162514264337593544

  s21_decimal decimal_check = {{0x8D36B4C8, 0xF7A0B5ED, 0x10C6, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_24) {
  // -79228162514264337593543.950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80060000}};

  // -79228162514264337593544

  s21_decimal decimal_check = {{0x8D36B4C8, 0xF7A0B5ED, 0x10C6, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_25) {
  // 7922816251426433759354.3950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x70000}};

  // 7922816251426433759354

  s21_decimal decimal_check = {{0xF485787A, 0x7F29ABCA, 0x1AD, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_26) {
  // -7922816251426433759354.3950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80070000}};

  // -7922816251426433759354

  s21_decimal decimal_check = {{0xF485787A, 0x7F29ABCA, 0x1AD, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_27) {
  // 792281625142643375935.43950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000}};

  // 792281625142643375935

  s21_decimal decimal_check = {{0x1873BF3F, 0xF31DC461, 0x2A, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_28) {
  // -792281625142643375935.43950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80080000}};

  // -792281625142643375935

  s21_decimal decimal_check = {{0x1873BF3F, 0xF31DC461, 0x2A, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_29) {
  // 79228162514264337593.543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x90000}};

  // 79228162514264337594

  s21_decimal decimal_check = {{0xB5A52CBA, 0x4B82FA09, 0x4, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_30) {
  // -79228162514264337593.543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80090000}};

  // -79228162514264337594

  s21_decimal decimal_check = {{0xB5A52CBA, 0x4B82FA09, 0x4, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_31) {
  // 7922816251426433759.3543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xA0000}};

  // 7922816251426433759

  s21_decimal decimal_check = {{0x5EF6EADF, 0x6DF37F67, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_32) {
  // -7922816251426433759.3543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x800A0000}};

  // -7922816251426433759

  s21_decimal decimal_check = {{0x5EF6EADF, 0x6DF37F67, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_33) {
  // 792281625142643375.93543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xB0000}};

  // 792281625142643376

  s21_decimal decimal_check = {{0xBCB24AB0, 0xAFEBFF0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_34) {
  // -792281625142643375.93543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x800B0000}};

  // -792281625142643376

  s21_decimal decimal_check = {{0xBCB24AB0, 0xAFEBFF0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_35) {
  // 79228162514264337.593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xC0000}};

  // 79228162514264338

  s21_decimal decimal_check = {{0x12DEA112, 0x1197998, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_36) {
  // -79228162514264337.593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x800C0000}};

  // -79228162514264338

  s21_decimal decimal_check = {{0x12DEA112, 0x1197998, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_37) {
  // 7922816251426433.7593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xD0000}};

  // 7922816251426434

  s21_decimal decimal_check = {{0x68497682, 0x1C25C2, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_38) {
  // -7922816251426433.7593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x800D0000}};

  // -7922816251426434

  s21_decimal decimal_check = {{0x68497682, 0x1C25C2, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_39) {
  // 792281625142643.37593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xE0000}};

  // 792281625142643

  s21_decimal decimal_check = {{0x70D42573, 0x2D093, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_40) {
  // -792281625142643.37593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x800E0000}};

  // -792281625142643

  s21_decimal decimal_check = {{0x70D42573, 0x2D093, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_41) {
  // 79228162514264.337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xF0000}};

  // 79228162514264

  s21_decimal decimal_check = {{0xBE7B9D58, 0x480E, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_42) {
  // -79228162514264.337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x800F0000}};

  // -79228162514264

  s21_decimal decimal_check = {{0xBE7B9D58, 0x480E, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_43) {
  // 7922816251426.4337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x100000}};

  // 7922816251426

  s21_decimal decimal_check = {{0xACA5F622, 0x734, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_44) {
  // -7922816251426.4337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80100000}};

  // -7922816251426

  s21_decimal decimal_check = {{0xACA5F622, 0x734, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_45) {
  // 792281625142.64337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x110000}};

  // 792281625143

  s21_decimal decimal_check = {{0x77AA3237, 0xB8, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_46) {
  // -792281625142.64337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80110000}};

  // -792281625143

  s21_decimal decimal_check = {{0x77AA3237, 0xB8, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_47) {
  // 79228162514.264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x120000}};

  // 79228162514

  s21_decimal decimal_check = {{0x725DD1D2, 0x12, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_48) {
  // -79228162514.264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80120000}};

  // -79228162514

  s21_decimal decimal_check = {{0x725DD1D2, 0x12, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_49) {
  // 7922816251.4264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x130000}};

  // 7922816251

  s21_decimal decimal_check = {{0xD83C94FB, 0x1, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_50) {
  // -7922816251.4264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80130000}};

  // -7922816251

  s21_decimal decimal_check = {{0xD83C94FB, 0x1, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_51) {
  // 792281625.14264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};

  // 792281625

  s21_decimal decimal_check = {{0x2F394219, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_52) {
  // -792281625.14264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80140000}};

  // -792281625

  s21_decimal decimal_check = {{0x2F394219, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_53) {
  // 79228162.514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x150000}};

  // 79228163

  s21_decimal decimal_check = {{0x4B8ED03, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_54) {
  // -79228162.514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80150000}};

  // -79228163

  s21_decimal decimal_check = {{0x4B8ED03, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_55) {
  // 7922816.2514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x160000}};

  // 7922816

  s21_decimal decimal_check = {{0x78E480, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_56) {
  // -7922816.2514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80160000}};

  // -7922816

  s21_decimal decimal_check = {{0x78E480, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_57) {
  // 792281.62514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x170000}};

  // 792282

  s21_decimal decimal_check = {{0xC16DA, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_58) {
  // -792281.62514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80170000}};

  // -792282

  s21_decimal decimal_check = {{0xC16DA, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_59) {
  // 79228.162514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x180000}};

  // 79228

  s21_decimal decimal_check = {{0x1357C, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_60) {
  // -79228.162514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80180000}};

  // -79228

  s21_decimal decimal_check = {{0x1357C, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_61) {
  // 7922.8162514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x190000}};

  // 7923

  s21_decimal decimal_check = {{0x1EF3, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_62) {
  // -7922.8162514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80190000}};

  // -7923

  s21_decimal decimal_check = {{0x1EF3, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_63) {
  // 792.28162514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1A0000}};

  // 792

  s21_decimal decimal_check = {{0x318, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_64) {
  // -792.28162514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x801A0000}};

  // -792

  s21_decimal decimal_check = {{0x318, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_65) {
  // 79.228162514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1B0000}};

  // 79

  s21_decimal decimal_check = {{0x4F, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_66) {
  // -79.228162514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x801B0000}};

  // -79

  s21_decimal decimal_check = {{0x4F, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_67) {
  // 7.9228162514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};

  // 8

  s21_decimal decimal_check = {{0x8, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_68) {
  // -7.9228162514264337593543950335

  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x801C0000}};

  // -8

  s21_decimal decimal_check = {{0x8, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_69) {
  // 79228162514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  // 79228162514264337593543950334

  s21_decimal decimal_check = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_70) {
  // -79228162514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  // -79228162514264337593543950334

  s21_decimal decimal_check = {

      {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_71) {
  // 7922816251426433759354395033.4

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}};

  // 7922816251426433759354395033

  s21_decimal decimal_check = {{0x99999999, 0x99999999, 0x19999999, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_72) {
  // -7922816251426433759354395033.4

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}};

  // -7922816251426433759354395033

  s21_decimal decimal_check = {

      {0x99999999, 0x99999999, 0x19999999, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_73) {
  // 792281625142643375935439503.34

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x20000}};

  // 792281625142643375935439503

  s21_decimal decimal_check = {{0x28F5C28F, 0xF5C28F5C, 0x28F5C28, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_74) {
  // -792281625142643375935439503.34

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80020000}};

  // -792281625142643375935439503

  s21_decimal decimal_check = {{0x28F5C28F, 0xF5C28F5C, 0x28F5C28, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_75) {
  // 79228162514264337593543950.334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x30000}};

  // 79228162514264337593543950

  s21_decimal decimal_check = {{0x9DB22D0E, 0x4BC6A7EF, 0x418937, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_76) {
  // -79228162514264337593543950.334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80030000}};

  // -79228162514264337593543950

  s21_decimal decimal_check = {{0x9DB22D0E, 0x4BC6A7EF, 0x418937, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_77) {
  // 7922816251426433759354395.0334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x40000}};

  // 7922816251426433759354395

  s21_decimal decimal_check = {{0x295E9E1B, 0xBAC710CB, 0x68DB8, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_78) {
  // -7922816251426433759354395.0334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80040000}};

  // -7922816251426433759354395

  s21_decimal decimal_check = {{0x295E9E1B, 0xBAC710CB, 0x68DB8, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_79) {
  // 792281625142643375935439.50334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x50000}};

  // 792281625142643375935440

  s21_decimal decimal_check = {{0x84230FD0, 0xAC471B47, 0xA7C5, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_80) {
  // -792281625142643375935439.50334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80050000}};

  // -792281625142643375935440

  s21_decimal decimal_check = {{0x84230FD0, 0xAC471B47, 0xA7C5, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_81) {
  // 79228162514264337593543.950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x60000}};

  // 79228162514264337593544

  s21_decimal decimal_check = {{0x8D36B4C8, 0xF7A0B5ED, 0x10C6, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_82) {
  // -79228162514264337593543.950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80060000}};

  // -79228162514264337593544

  s21_decimal decimal_check = {{0x8D36B4C8, 0xF7A0B5ED, 0x10C6, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_83) {
  // 7922816251426433759354.3950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x70000}};

  // 7922816251426433759354

  s21_decimal decimal_check = {{0xF485787A, 0x7F29ABCA, 0x1AD, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_84) {
  // -7922816251426433759354.3950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80070000}};

  // -7922816251426433759354

  s21_decimal decimal_check = {{0xF485787A, 0x7F29ABCA, 0x1AD, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_85) {
  // 792281625142643375935.43950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000}};

  // 792281625142643375935

  s21_decimal decimal_check = {{0x1873BF3F, 0xF31DC461, 0x2A, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_86) {
  // -792281625142643375935.43950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80080000}};

  // -792281625142643375935

  s21_decimal decimal_check = {{0x1873BF3F, 0xF31DC461, 0x2A, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_87) {
  // 79228162514264337593.543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x90000}};

  // 79228162514264337594

  s21_decimal decimal_check = {{0xB5A52CBA, 0x4B82FA09, 0x4, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_88) {
  // -79228162514264337593.543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80090000}};

  // -79228162514264337594

  s21_decimal decimal_check = {{0xB5A52CBA, 0x4B82FA09, 0x4, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_89) {
  // 7922816251426433759.3543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xA0000}};

  // 7922816251426433759

  s21_decimal decimal_check = {{0x5EF6EADF, 0x6DF37F67, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_90) {
  // -7922816251426433759.3543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x800A0000}};

  // -7922816251426433759

  s21_decimal decimal_check = {{0x5EF6EADF, 0x6DF37F67, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_91) {
  // 792281625142643375.93543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xB0000}};

  // 792281625142643376

  s21_decimal decimal_check = {{0xBCB24AB0, 0xAFEBFF0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_92) {
  // -792281625142643375.93543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x800B0000}};

  // -792281625142643376

  s21_decimal decimal_check = {{0xBCB24AB0, 0xAFEBFF0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_93) {
  // 79228162514264337.593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xC0000}};

  // 79228162514264338

  s21_decimal decimal_check = {{0x12DEA112, 0x1197998, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_94) {
  // -79228162514264337.593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x800C0000}};

  // -79228162514264338

  s21_decimal decimal_check = {{0x12DEA112, 0x1197998, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_95) {
  // 7922816251426433.7593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xD0000}};

  // 7922816251426434

  s21_decimal decimal_check = {{0x68497682, 0x1C25C2, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_96) {
  // -7922816251426433.7593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x800D0000}};

  // -7922816251426434

  s21_decimal decimal_check = {{0x68497682, 0x1C25C2, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_97) {
  // 792281625142643.37593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xE0000}};

  // 792281625142643

  s21_decimal decimal_check = {{0x70D42573, 0x2D093, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_98) {
  // -792281625142643.37593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x800E0000}};

  // -792281625142643

  s21_decimal decimal_check = {{0x70D42573, 0x2D093, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_99) {
  // 79228162514264.337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xF0000}};

  // 79228162514264

  s21_decimal decimal_check = {{0xBE7B9D58, 0x480E, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_100) {
  // -79228162514264.337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x800F0000}};

  // -79228162514264

  s21_decimal decimal_check = {{0xBE7B9D58, 0x480E, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_101) {
  // 7922816251426.4337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x100000}};

  // 7922816251426

  s21_decimal decimal_check = {{0xACA5F622, 0x734, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_102) {
  // -7922816251426.4337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80100000}};

  // -7922816251426

  s21_decimal decimal_check = {{0xACA5F622, 0x734, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_103) {
  // 792281625142.64337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x110000}};

  // 792281625143

  s21_decimal decimal_check = {{0x77AA3237, 0xB8, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_104) {
  // -792281625142.64337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80110000}};

  // -792281625143

  s21_decimal decimal_check = {{0x77AA3237, 0xB8, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_105) {
  // 79228162514.264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x120000}};

  // 79228162514

  s21_decimal decimal_check = {{0x725DD1D2, 0x12, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_106) {
  // -79228162514.264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80120000}};

  // -79228162514

  s21_decimal decimal_check = {{0x725DD1D2, 0x12, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_107) {
  // 7922816251.4264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x130000}};

  // 7922816251

  s21_decimal decimal_check = {{0xD83C94FB, 0x1, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_108) {
  // -7922816251.4264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80130000}};

  // -7922816251

  s21_decimal decimal_check = {{0xD83C94FB, 0x1, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_109) {
  // 792281625.14264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};

  // 792281625

  s21_decimal decimal_check = {{0x2F394219, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_110) {
  // -792281625.14264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80140000}};

  // -792281625

  s21_decimal decimal_check = {{0x2F394219, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_111) {
  // 79228162.514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x150000}};

  // 79228163

  s21_decimal decimal_check = {{0x4B8ED03, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_112) {
  // -79228162.514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80150000}};

  // -79228163

  s21_decimal decimal_check = {{0x4B8ED03, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_113) {
  // 7922816.2514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x160000}};

  // 7922816

  s21_decimal decimal_check = {{0x78E480, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_114) {
  // -7922816.2514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80160000}};

  // -7922816

  s21_decimal decimal_check = {{0x78E480, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_115) {
  // 792281.62514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x170000}};

  // 792282

  s21_decimal decimal_check = {{0xC16DA, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_116) {
  // -792281.62514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80170000}};

  // -792282

  s21_decimal decimal_check = {{0xC16DA, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_117) {
  // 79228.162514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x180000}};

  // 79228

  s21_decimal decimal_check = {{0x1357C, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_118) {
  // -79228.162514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80180000}};

  // -79228

  s21_decimal decimal_check = {{0x1357C, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_119) {
  // 7922.8162514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x190000}};

  // 7923

  s21_decimal decimal_check = {{0x1EF3, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_120) {
  // -7922.8162514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80190000}};

  // -7923

  s21_decimal decimal_check = {{0x1EF3, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_121) {
  // 792.28162514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x1A0000}};

  // 792

  s21_decimal decimal_check = {{0x318, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_122) {
  // -792.28162514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x801A0000}};

  // -792

  s21_decimal decimal_check = {{0x318, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_123) {
  // 79.228162514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x1B0000}};

  // 79

  s21_decimal decimal_check = {{0x4F, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_124) {
  // -79.228162514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x801B0000}};

  // -79

  s21_decimal decimal_check = {{0x4F, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_125) {
  // 7.9228162514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};

  // 8

  s21_decimal decimal_check = {{0x8, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_126) {
  // -7.9228162514264337593543950334

  s21_decimal decimal = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x801C0000}};

  // -8

  s21_decimal decimal_check = {{0x8, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_127) {
  // 52818775009509558395695966890

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0x0}};

  // 52818775009509558395695966890

  s21_decimal decimal_check = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_128) {
  // -52818775009509558395695966890

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0x80000000}};

  // -52818775009509558395695966890

  s21_decimal decimal_check = {

      {0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_129) {
  // 5281877500950955839569596689.0

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0x10000}};

  // 5281877500950955839569596689

  s21_decimal decimal_check = {{0x11111111, 0x11111111, 0x11111111, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_130) {
  // -5281877500950955839569596689.0

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0x80010000}};

  // -5281877500950955839569596689

  s21_decimal decimal_check = {

      {0x11111111, 0x11111111, 0x11111111, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_131) {
  // 5281877500950955.8395695966890

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0xD0000}};

  // 5281877500950956

  s21_decimal decimal_check = {{0xF030F9AC, 0x12C3D6, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_132) {
  // -5281877500950955.8395695966890

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0x800D0000}};

  // -5281877500950956

  s21_decimal decimal_check = {{0xF030F9AC, 0x12C3D6, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_133) {
  // 5.2818775009509558395695966890

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0x1C0000}};

  // 5

  s21_decimal decimal_check = {{0x5, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_134) {
  // -5.2818775009509558395695966890

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0xAAAAAAAA, 0x801C0000}};

  // -5

  s21_decimal decimal_check = {{0x5, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_135) {
  // 52818775009509558392832655360

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0xAAAAAAAA, 0x0}};

  // 52818775009509558392832655360

  s21_decimal decimal_check = {{0x0, 0xAAAAAAAA, 0xAAAAAAAA, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_136) {
  // -52818775009509558392832655360

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0xAAAAAAAA, 0x80000000}};

  // -52818775009509558392832655360

  s21_decimal decimal_check = {{0x0, 0xAAAAAAAA, 0xAAAAAAAA, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_137) {
  // 5281877500950955839283265536.0

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0xAAAAAAAA, 0x10000}};

  // 5281877500950955839283265536

  s21_decimal decimal_check = {{0x0, 0x11111111, 0x11111111, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_138) {
  // -5281877500950955839283265536.0

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0xAAAAAAAA, 0x80010000}};

  // -5281877500950955839283265536

  s21_decimal decimal_check = {{0x0, 0x11111111, 0x11111111, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_139) {
  // 5281877500950955.8392832655360

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0xAAAAAAAA, 0xD0000}};

  // 5281877500950956

  s21_decimal decimal_check = {{0xF030F9AC, 0x12C3D6, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_140) {
  // -5281877500950955.8392832655360

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0xAAAAAAAA, 0x800D0000}};

  // -5281877500950956

  s21_decimal decimal_check = {{0xF030F9AC, 0x12C3D6, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_141) {
  // 5.2818775009509558392832655360

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0xAAAAAAAA, 0x1C0000}};

  // 5

  s21_decimal decimal_check = {{0x5, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_142) {
  // -5.2818775009509558392832655360

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0xAAAAAAAA, 0x801C0000}};

  // -5

  s21_decimal decimal_check = {{0x5, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_143) {
  // 52818774997211729016086244010

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0xAAAAAAAA, 0x0}};

  // 52818774997211729016086244010

  s21_decimal decimal_check = {{0xAAAAAAAA, 0x0, 0xAAAAAAAA, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_144) {
  // -52818774997211729016086244010

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0xAAAAAAAA, 0x80000000}};

  // -52818774997211729016086244010

  s21_decimal decimal_check = {{0xAAAAAAAA, 0x0, 0xAAAAAAAA, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_145) {
  // 5281877499721172901608624401.0

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0xAAAAAAAA, 0x10000}};

  // 5281877499721172901608624401

  s21_decimal decimal_check = {{0x11111111, 0x0, 0x11111111, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_146) {
  // -5281877499721172901608624401.0

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0xAAAAAAAA, 0x80010000}};

  // -5281877499721172901608624401

  s21_decimal decimal_check = {{0x11111111, 0x0, 0x11111111, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_147) {
  // 5281877499721172.9016086244010

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0xAAAAAAAA, 0xD0000}};

  // 5281877499721173

  s21_decimal decimal_check = {{0xF01E35D5, 0x12C3D6, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_148) {
  // -5281877499721172.9016086244010

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0xAAAAAAAA, 0x800D0000}};

  // -5281877499721173

  s21_decimal decimal_check = {{0xF01E35D5, 0x12C3D6, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_149) {
  // 5.2818774997211729016086244010

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0xAAAAAAAA, 0x1C0000}};

  // 5

  s21_decimal decimal_check = {{0x5, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_150) {
  // -5.2818774997211729016086244010

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0xAAAAAAAA, 0x801C0000}};

  // -5

  s21_decimal decimal_check = {{0x5, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_151) {
  // 52818774997211729013222932480

  s21_decimal decimal = {{0x0, 0x0, 0xAAAAAAAA, 0x0}};

  // 52818774997211729013222932480

  s21_decimal decimal_check = {{0x0, 0x0, 0xAAAAAAAA, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_152) {
  // -52818774997211729013222932480

  s21_decimal decimal = {{0x0, 0x0, 0xAAAAAAAA, 0x80000000}};

  // -52818774997211729013222932480

  s21_decimal decimal_check = {{0x0, 0x0, 0xAAAAAAAA, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_153) {
  // 5281877499721172901322293248.0

  s21_decimal decimal = {{0x0, 0x0, 0xAAAAAAAA, 0x10000}};

  // 5281877499721172901322293248

  s21_decimal decimal_check = {{0x0, 0x0, 0x11111111, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_154) {
  // -5281877499721172901322293248.0

  s21_decimal decimal = {{0x0, 0x0, 0xAAAAAAAA, 0x80010000}};

  // -5281877499721172901322293248

  s21_decimal decimal_check = {{0x0, 0x0, 0x11111111, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_155) {
  // 5281877499721172.9013222932480

  s21_decimal decimal = {{0x0, 0x0, 0xAAAAAAAA, 0xD0000}};

  // 5281877499721173

  s21_decimal decimal_check = {{0xF01E35D5, 0x12C3D6, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_156) {
  // -5281877499721172.9013222932480

  s21_decimal decimal = {{0x0, 0x0, 0xAAAAAAAA, 0x800D0000}};

  // -5281877499721173

  s21_decimal decimal_check = {{0xF01E35D5, 0x12C3D6, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_157) {
  // 5.2818774997211729013222932480

  s21_decimal decimal = {{0x0, 0x0, 0xAAAAAAAA, 0x1C0000}};

  // 5

  s21_decimal decimal_check = {{0x5, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_158) {
  // -5.2818774997211729013222932480

  s21_decimal decimal = {{0x0, 0x0, 0xAAAAAAAA, 0x801C0000}};

  // -5

  s21_decimal decimal_check = {{0x5, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_159) {
  // 12297829382473034410

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0x0, 0x0}};

  // 12297829382473034410

  s21_decimal decimal_check = {{0xAAAAAAAA, 0xAAAAAAAA, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_160) {
  // -12297829382473034410

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0x0, 0x80000000}};

  // -12297829382473034410

  s21_decimal decimal_check = {{0xAAAAAAAA, 0xAAAAAAAA, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_161) {
  // 1229782938247303441.0

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0x0, 0x10000}};

  // 1229782938247303441

  s21_decimal decimal_check = {{0x11111111, 0x11111111, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_162) {
  // -1229782938247303441.0

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0x0, 0x80010000}};

  // -1229782938247303441

  s21_decimal decimal_check = {{0x11111111, 0x11111111, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_163) {
  // 122978293.82473034410

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0x0, 0xB0000}};

  // 122978294

  s21_decimal decimal_check = {{0x7547FF6, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_164) {
  // -122978293.82473034410

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0x0, 0x800B0000}};

  // -122978294

  s21_decimal decimal_check = {{0x7547FF6, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_165) {
  // 1.2297829382473034410

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0x0, 0x130000}};

  // 1

  s21_decimal decimal_check = {{0x1, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_166) {
  // -1.2297829382473034410

  s21_decimal decimal = {{0xAAAAAAAA, 0xAAAAAAAA, 0x0, 0x80130000}};

  // -1

  s21_decimal decimal_check = {{0x1, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_167) {
  // 12297829379609722880

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0x0, 0x0}};

  // 12297829379609722880

  s21_decimal decimal_check = {{0x0, 0xAAAAAAAA, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_168) {
  // -12297829379609722880

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0x0, 0x80000000}};

  // -12297829379609722880

  s21_decimal decimal_check = {{0x0, 0xAAAAAAAA, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_169) {
  // 1229782937960972288.0

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0x0, 0x10000}};

  // 1229782937960972288

  s21_decimal decimal_check = {{0x0, 0x11111111, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_170) {
  // -1229782937960972288.0

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0x0, 0x80010000}};

  // -1229782937960972288

  s21_decimal decimal_check = {{0x0, 0x11111111, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_171) {
  // 122978293.79609722880

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0x0, 0xB0000}};

  // 122978294

  s21_decimal decimal_check = {{0x7547FF6, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_172) {
  // -122978293.79609722880

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0x0, 0x800B0000}};

  // -122978294

  s21_decimal decimal_check = {{0x7547FF6, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_173) {
  // 1.2297829379609722880

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0x0, 0x130000}};

  // 1

  s21_decimal decimal_check = {{0x1, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_174) {
  // -1.2297829379609722880

  s21_decimal decimal = {{0x0, 0xAAAAAAAA, 0x0, 0x80130000}};

  // -1

  s21_decimal decimal_check = {{0x1, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_175) {
  // 2863311530

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0x0, 0x0}};

  // 2863311530

  s21_decimal decimal_check = {{0xAAAAAAAA, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_176) {
  // -2863311530

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0x0, 0x80000000}};

  // -2863311530

  s21_decimal decimal_check = {{0xAAAAAAAA, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_177) {
  // 286331153.0

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0x0, 0x10000}};

  // 286331153

  s21_decimal decimal_check = {{0x11111111, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_178) {
  // -286331153.0

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0x0, 0x80010000}};

  // -286331153

  s21_decimal decimal_check = {{0x11111111, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_179) {
  // 286331.1530

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0x0, 0x40000}};

  // 286331

  s21_decimal decimal_check = {{0x45E7B, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_180) {
  // -286331.1530

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0x0, 0x80040000}};

  // -286331

  s21_decimal decimal_check = {{0x45E7B, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_181) {
  // 2.863311530

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0x0, 0x90000}};

  // 3

  s21_decimal decimal_check = {{0x3, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_182) {
  // -2.863311530

  s21_decimal decimal = {{0xAAAAAAAA, 0x0, 0x0, 0x80090000}};

  // -3

  s21_decimal decimal_check = {{0x3, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_183) {
  // 26409387504754779197847983445

  s21_decimal decimal = {{0x55555555, 0x55555555, 0x55555555, 0x0}};

  // 26409387504754779197847983445

  s21_decimal decimal_check = {{0x55555555, 0x55555555, 0x55555555, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_184) {
  // -26409387504754779197847983445

  s21_decimal decimal = {{0x55555555, 0x55555555, 0x55555555, 0x80000000}};

  // -26409387504754779197847983445

  s21_decimal decimal_check = {

      {0x55555555, 0x55555555, 0x55555555, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_185) {
  // 2640938750475477919784798344.5

  s21_decimal decimal = {{0x55555555, 0x55555555, 0x55555555, 0x10000}};

  // 2640938750475477919784798344

  s21_decimal decimal_check = {{0x88888889, 0x88888888, 0x8888888, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_186) {
  // -2640938750475477919784798344.5

  s21_decimal decimal = {{0x55555555, 0x55555555, 0x55555555, 0x80010000}};

  // -2640938750475477919784798344

  s21_decimal decimal_check = {{0x88888889, 0x88888888, 0x8888888, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_187) {
  // 2640938750475477.9197847983445

  s21_decimal decimal = {{0x55555555, 0x55555555, 0x55555555, 0xD0000}};

  // 2640938750475478

  s21_decimal decimal_check = {{0x78187CD6, 0x961EB, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_188) {
  // -2640938750475477.9197847983445

  s21_decimal decimal = {{0x55555555, 0x55555555, 0x55555555, 0x800D0000}};

  // -2640938750475478

  s21_decimal decimal_check = {{0x78187CD6, 0x961EB, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_189) {
  // 2.6409387504754779197847983445

  s21_decimal decimal = {{0x55555555, 0x55555555, 0x55555555, 0x1C0000}};

  // 3

  s21_decimal decimal_check = {{0x3, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_190) {
  // -2.6409387504754779197847983445

  s21_decimal decimal = {{0x55555555, 0x55555555, 0x55555555, 0x801C0000}};

  // -3

  s21_decimal decimal_check = {{0x3, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_191) {
  // 26409387504754779196416327680

  s21_decimal decimal = {{0x0, 0x55555555, 0x55555555, 0x0}};

  // 26409387504754779196416327680

  s21_decimal decimal_check = {{0x0, 0x55555555, 0x55555555, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_192) {
  // -26409387504754779196416327680

  s21_decimal decimal = {{0x0, 0x55555555, 0x55555555, 0x80000000}};

  // -26409387504754779196416327680

  s21_decimal decimal_check = {{0x0, 0x55555555, 0x55555555, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_193) {
  // 2640938750475477919641632768.0

  s21_decimal decimal = {{0x0, 0x55555555, 0x55555555, 0x10000}};

  // 2640938750475477919641632768

  s21_decimal decimal_check = {{0x80000000, 0x88888888, 0x8888888, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_194) {
  // -2640938750475477919641632768.0

  s21_decimal decimal = {{0x0, 0x55555555, 0x55555555, 0x80010000}};

  // -2640938750475477919641632768

  s21_decimal decimal_check = {{0x80000000, 0x88888888, 0x8888888, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_195) {
  // 2640938750475477.9196416327680

  s21_decimal decimal = {{0x0, 0x55555555, 0x55555555, 0xD0000}};

  // 2640938750475478

  s21_decimal decimal_check = {{0x78187CD6, 0x961EB, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_196) {
  // -2640938750475477.9196416327680

  s21_decimal decimal = {{0x0, 0x55555555, 0x55555555, 0x800D0000}};

  // -2640938750475478

  s21_decimal decimal_check = {{0x78187CD6, 0x961EB, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_197) {
  // 2.6409387504754779196416327680

  s21_decimal decimal = {{0x0, 0x55555555, 0x55555555, 0x1C0000}};

  // 3

  s21_decimal decimal_check = {{0x3, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_198) {
  // -2.6409387504754779196416327680

  s21_decimal decimal = {{0x0, 0x55555555, 0x55555555, 0x801C0000}};

  // -3

  s21_decimal decimal_check = {{0x3, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_199) {
  // 26409387498605864508043122005

  s21_decimal decimal = {{0x55555555, 0x0, 0x55555555, 0x0}};

  // 26409387498605864508043122005

  s21_decimal decimal_check = {{0x55555555, 0x0, 0x55555555, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_200) {
  // -26409387498605864508043122005

  s21_decimal decimal = {{0x55555555, 0x0, 0x55555555, 0x80000000}};

  // -26409387498605864508043122005

  s21_decimal decimal_check = {{0x55555555, 0x0, 0x55555555, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_201) {
  // 2640938749860586450804312200.5

  s21_decimal decimal = {{0x55555555, 0x0, 0x55555555, 0x10000}};

  // 2640938749860586450804312200

  s21_decimal decimal_check = {{0x8888889, 0x80000000, 0x8888888, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_202) {
  // -2640938749860586450804312200.5

  s21_decimal decimal = {{0x55555555, 0x0, 0x55555555, 0x80010000}};

  // -2640938749860586450804312200

  s21_decimal decimal_check = {{0x8888889, 0x80000000, 0x8888888, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_203) {
  // 2640938749860586.4508043122005

  s21_decimal decimal = {{0x55555555, 0x0, 0x55555555, 0xD0000}};

  // 2640938749860586

  s21_decimal decimal_check = {{0x780F1AEA, 0x961EB, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_204) {
  // -2640938749860586.4508043122005

  s21_decimal decimal = {{0x55555555, 0x0, 0x55555555, 0x800D0000}};

  // -2640938749860586

  s21_decimal decimal_check = {{0x780F1AEA, 0x961EB, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_205) {
  // 2.6409387498605864508043122005

  s21_decimal decimal = {{0x55555555, 0x0, 0x55555555, 0x1C0000}};

  // 3

  s21_decimal decimal_check = {{0x3, 0x0, 0x0, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_206) {
  // -2.6409387498605864508043122005

  s21_decimal decimal = {{0x55555555, 0x0, 0x55555555, 0x801C0000}};

  // -3

  s21_decimal decimal_check = {{0x3, 0x0, 0x0, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_207) {
  // 26409387498605864506611466240

  s21_decimal decimal = {{0x0, 0x0, 0x55555555, 0x0}};

  // 26409387498605864506611466240

  s21_decimal decimal_check = {{0x0, 0x0, 0x55555555, 0x0}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_208) {
  // -26409387498605864506611466240

  s21_decimal decimal = {{0x0, 0x0, 0x55555555, 0x80000000}};

  // -26409387498605864506611466240

  s21_decimal decimal_check = {{0x0, 0x0, 0x55555555, 0x80000000}};

  test_round_f(decimal, decimal_check);
}

START_TEST(test_round_209) {
  // 2640938749860586450661146624.0

  s21_decimal decimal = {{0x0, 0x0, 0x55555555, 0x10000}};

  // 2640938749860586450661146624

  s21_decimal decimal_check = {{0x0, 0x80000000, 0x8888888, 0x0}};

  test_round_f(decimal, decimal_check);
}

Suite *test_round(void) {
  Suite *s;

  TCase *tc;

  s = suite_create("\033[42m\033[30m========= ROUND =========\033[0m");

  tc = tcase_create("s21_round");

  tcase_add_test(tc, test_round_1);

  tcase_add_test(tc, test_round_2);

  tcase_add_test(tc, test_round_3);

  tcase_add_test(tc, test_round_4);

  tcase_add_test(tc, test_round_5);

  tcase_add_test(tc, test_round_6);

  tcase_add_test(tc, test_round_7);

  tcase_add_test(tc, test_round_8);

  tcase_add_test(tc, test_round_9);

  tcase_add_test(tc, test_round_10);

  tcase_add_test(tc, test_round_11);

  tcase_add_test(tc, test_round_12);

  tcase_add_test(tc, test_round_13);

  tcase_add_test(tc, test_round_14);

  tcase_add_test(tc, test_round_15);

  tcase_add_test(tc, test_round_16);

  tcase_add_test(tc, test_round_17);

  tcase_add_test(tc, test_round_18);

  tcase_add_test(tc, test_round_19);

  tcase_add_test(tc, test_round_20);

  tcase_add_test(tc, test_round_21);

  tcase_add_test(tc, test_round_22);

  tcase_add_test(tc, test_round_23);

  tcase_add_test(tc, test_round_24);

  tcase_add_test(tc, test_round_25);

  tcase_add_test(tc, test_round_26);

  tcase_add_test(tc, test_round_27);

  tcase_add_test(tc, test_round_28);

  tcase_add_test(tc, test_round_29);

  tcase_add_test(tc, test_round_30);

  tcase_add_test(tc, test_round_31);

  tcase_add_test(tc, test_round_32);

  tcase_add_test(tc, test_round_33);

  tcase_add_test(tc, test_round_34);

  tcase_add_test(tc, test_round_35);

  tcase_add_test(tc, test_round_36);

  tcase_add_test(tc, test_round_37);

  tcase_add_test(tc, test_round_38);

  tcase_add_test(tc, test_round_39);

  tcase_add_test(tc, test_round_40);

  tcase_add_test(tc, test_round_41);

  tcase_add_test(tc, test_round_42);

  tcase_add_test(tc, test_round_43);

  tcase_add_test(tc, test_round_44);

  tcase_add_test(tc, test_round_45);

  tcase_add_test(tc, test_round_46);

  tcase_add_test(tc, test_round_47);

  tcase_add_test(tc, test_round_48);

  tcase_add_test(tc, test_round_49);

  tcase_add_test(tc, test_round_50);

  tcase_add_test(tc, test_round_51);

  tcase_add_test(tc, test_round_52);

  tcase_add_test(tc, test_round_53);

  tcase_add_test(tc, test_round_54);

  tcase_add_test(tc, test_round_55);

  tcase_add_test(tc, test_round_56);

  tcase_add_test(tc, test_round_57);

  tcase_add_test(tc, test_round_58);

  tcase_add_test(tc, test_round_59);

  tcase_add_test(tc, test_round_60);

  tcase_add_test(tc, test_round_61);

  tcase_add_test(tc, test_round_62);

  tcase_add_test(tc, test_round_63);

  tcase_add_test(tc, test_round_64);

  tcase_add_test(tc, test_round_65);

  tcase_add_test(tc, test_round_66);

  tcase_add_test(tc, test_round_67);

  tcase_add_test(tc, test_round_68);

  tcase_add_test(tc, test_round_69);

  tcase_add_test(tc, test_round_70);

  tcase_add_test(tc, test_round_71);

  tcase_add_test(tc, test_round_72);

  tcase_add_test(tc, test_round_73);

  tcase_add_test(tc, test_round_74);

  tcase_add_test(tc, test_round_75);

  tcase_add_test(tc, test_round_76);

  tcase_add_test(tc, test_round_77);

  tcase_add_test(tc, test_round_78);

  tcase_add_test(tc, test_round_79);

  tcase_add_test(tc, test_round_80);

  tcase_add_test(tc, test_round_81);

  tcase_add_test(tc, test_round_82);

  tcase_add_test(tc, test_round_83);

  tcase_add_test(tc, test_round_84);

  tcase_add_test(tc, test_round_85);

  tcase_add_test(tc, test_round_86);

  tcase_add_test(tc, test_round_87);

  tcase_add_test(tc, test_round_88);

  tcase_add_test(tc, test_round_89);

  tcase_add_test(tc, test_round_90);

  tcase_add_test(tc, test_round_91);

  tcase_add_test(tc, test_round_92);

  tcase_add_test(tc, test_round_93);

  tcase_add_test(tc, test_round_94);

  tcase_add_test(tc, test_round_95);

  tcase_add_test(tc, test_round_96);

  tcase_add_test(tc, test_round_97);

  tcase_add_test(tc, test_round_98);

  tcase_add_test(tc, test_round_99);

  tcase_add_test(tc, test_round_100);

  tcase_add_test(tc, test_round_101);

  tcase_add_test(tc, test_round_102);

  tcase_add_test(tc, test_round_103);

  tcase_add_test(tc, test_round_104);

  tcase_add_test(tc, test_round_105);

  tcase_add_test(tc, test_round_106);

  tcase_add_test(tc, test_round_107);

  tcase_add_test(tc, test_round_108);

  tcase_add_test(tc, test_round_109);

  tcase_add_test(tc, test_round_110);

  tcase_add_test(tc, test_round_111);

  tcase_add_test(tc, test_round_112);

  tcase_add_test(tc, test_round_113);

  tcase_add_test(tc, test_round_114);

  tcase_add_test(tc, test_round_115);

  tcase_add_test(tc, test_round_116);

  tcase_add_test(tc, test_round_117);

  tcase_add_test(tc, test_round_118);

  tcase_add_test(tc, test_round_119);

  tcase_add_test(tc, test_round_120);

  tcase_add_test(tc, test_round_121);

  tcase_add_test(tc, test_round_122);

  tcase_add_test(tc, test_round_123);

  tcase_add_test(tc, test_round_124);

  tcase_add_test(tc, test_round_125);

  tcase_add_test(tc, test_round_126);

  tcase_add_test(tc, test_round_127);

  tcase_add_test(tc, test_round_128);

  tcase_add_test(tc, test_round_129);

  tcase_add_test(tc, test_round_130);

  tcase_add_test(tc, test_round_131);

  tcase_add_test(tc, test_round_132);

  tcase_add_test(tc, test_round_133);

  tcase_add_test(tc, test_round_134);

  tcase_add_test(tc, test_round_135);

  tcase_add_test(tc, test_round_136);

  tcase_add_test(tc, test_round_137);

  tcase_add_test(tc, test_round_138);

  tcase_add_test(tc, test_round_139);

  tcase_add_test(tc, test_round_140);

  tcase_add_test(tc, test_round_141);

  tcase_add_test(tc, test_round_142);

  tcase_add_test(tc, test_round_143);

  tcase_add_test(tc, test_round_144);

  tcase_add_test(tc, test_round_145);

  tcase_add_test(tc, test_round_146);

  tcase_add_test(tc, test_round_147);

  tcase_add_test(tc, test_round_148);

  tcase_add_test(tc, test_round_149);

  tcase_add_test(tc, test_round_150);

  tcase_add_test(tc, test_round_151);

  tcase_add_test(tc, test_round_152);

  tcase_add_test(tc, test_round_153);

  tcase_add_test(tc, test_round_154);

  tcase_add_test(tc, test_round_155);

  tcase_add_test(tc, test_round_156);

  tcase_add_test(tc, test_round_157);

  tcase_add_test(tc, test_round_158);

  tcase_add_test(tc, test_round_159);

  tcase_add_test(tc, test_round_160);

  tcase_add_test(tc, test_round_161);

  tcase_add_test(tc, test_round_162);

  tcase_add_test(tc, test_round_163);

  tcase_add_test(tc, test_round_164);

  tcase_add_test(tc, test_round_165);

  tcase_add_test(tc, test_round_166);

  tcase_add_test(tc, test_round_167);

  tcase_add_test(tc, test_round_168);

  tcase_add_test(tc, test_round_169);

  tcase_add_test(tc, test_round_170);

  tcase_add_test(tc, test_round_171);

  tcase_add_test(tc, test_round_172);

  tcase_add_test(tc, test_round_173);

  tcase_add_test(tc, test_round_174);

  tcase_add_test(tc, test_round_175);

  tcase_add_test(tc, test_round_176);

  tcase_add_test(tc, test_round_177);

  tcase_add_test(tc, test_round_178);

  tcase_add_test(tc, test_round_179);

  tcase_add_test(tc, test_round_180);

  tcase_add_test(tc, test_round_181);

  tcase_add_test(tc, test_round_182);

  tcase_add_test(tc, test_round_183);

  tcase_add_test(tc, test_round_184);

  tcase_add_test(tc, test_round_185);

  tcase_add_test(tc, test_round_186);

  tcase_add_test(tc, test_round_187);

  tcase_add_test(tc, test_round_188);

  tcase_add_test(tc, test_round_189);

  tcase_add_test(tc, test_round_190);

  tcase_add_test(tc, test_round_191);

  tcase_add_test(tc, test_round_192);

  tcase_add_test(tc, test_round_193);

  tcase_add_test(tc, test_round_194);

  tcase_add_test(tc, test_round_195);

  tcase_add_test(tc, test_round_196);

  tcase_add_test(tc, test_round_197);

  tcase_add_test(tc, test_round_198);

  tcase_add_test(tc, test_round_199);

  tcase_add_test(tc, test_round_200);

  tcase_add_test(tc, test_round_201);

  tcase_add_test(tc, test_round_202);

  tcase_add_test(tc, test_round_203);

  tcase_add_test(tc, test_round_204);

  tcase_add_test(tc, test_round_205);

  tcase_add_test(tc, test_round_206);

  tcase_add_test(tc, test_round_207);

  tcase_add_test(tc, test_round_208);

  tcase_add_test(tc, test_round_209);

  suite_add_tcase(s, tc);

  return s;
}
