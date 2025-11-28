#ifndef __S21_MATRIX_H__
#define __S21_MATRIX_H__

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define FAILURE 0
#define SUCCESS 1

enum work_res { OK, INCORRECT_MATRIX, CALCULATION_ERR };

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

// Создание матрицы
int s21_create_matrix(int rows, int columns, matrix_t *result);

// Очистка матрицы (освобождение памяти)
void s21_remove_matrix(matrix_t *A);

// Сравнение матриц: возвращает 1, если матрицы равны (до 7 знаков после
// запятой), иначе 0
int s21_eq_matrix(matrix_t *A, matrix_t *B);

// Сложение матриц
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

// Вычитание матриц
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

// Умножение матрицы на число
int s21_mult_number(matrix_t *A, double number, matrix_t *result);

// Умножение двух матриц
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);

// Транспонирование матрицы
int s21_transpose(matrix_t *A, matrix_t *result);

// Вычисление матрицы алгебраических дополнений
int s21_calc_complements(matrix_t *A, matrix_t *result);

// Вычисление определителя матрицы
int s21_determinant(matrix_t *A, double *result);

// Вычисление обратной матрицы
int s21_inverse_matrix(matrix_t *A, matrix_t *result);

// Дополнительные функции
int my_create_matrix(int rows, int columns, double ***matrix);
void destroy(double **matrix, int rows);
int is_matrix_incorrect(matrix_t *A);
int copy_matrix(matrix_t *A, matrix_t *result);
int is_matrix_correct_for_mul(matrix_t *A, matrix_t *B);
void mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
void fill_minor(matrix_t *minor, int rows, int columns, matrix_t *A);
int is_matrix_square(matrix_t *A);
int is_almost_zero(double value);
int inverse_matrix(matrix_t *A, double determinant, matrix_t *result);

#endif  // __S21_MATRIX_H__
