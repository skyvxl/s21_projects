#ifndef __S21_TEST_MATRIX_H__
#define __S21_TEST_MATRIX_H__

#include <check.h>
#include <limits.h>

#include "../s21_matrix.h"

Suite *test_s21_create_matrix(void);
Suite *test_s21_remove_matrix(void);
Suite *test_s21_eq_matrix(void);
Suite *test_s21_sum_matrix(void);
Suite *test_s21_sub_matrix(void);
Suite *test_s21_mult_number(void);
Suite *test_s21_mult_matrix(void);
Suite *test_s21_transpose(void);
Suite *test_s21_calc_complements(void);
Suite *test_s21_determinant(void);
Suite *test_s21_inverse_matrix(void);

void init_matrix(matrix_t *Matrix, double const *values, size_t length);

#endif  // __S21_TEST_MATRIX_H__