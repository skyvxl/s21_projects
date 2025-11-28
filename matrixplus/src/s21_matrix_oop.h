#ifndef S21_MATRIX_OOP_H_
#define S21_MATRIX_OOP_H_

#include <cmath>
#include <stdexcept>

class S21Matrix {
 private:
  int rows_, cols_;  // Количество строк и столбцов
  double** matrix_;  // Указатель на память с матрицей

  // Валидация матрицы
  void CheckMatrixForValidity() const;

 public:
  // Конструкторы и деструктор
  S21Matrix();                            // Базовый конструктор
  S21Matrix(int rows, int cols);          // Конструктор с параметрами
  S21Matrix(const S21Matrix& other);      // Конструктор копирования
  S21Matrix(S21Matrix&& other) noexcept;  // Конструктор переноса
  ~S21Matrix();                           // Деструктор

  // Геттеры и сеттеры
  int GetRows() const;
  int GetCols() const;
  void SetRows(int rows);
  void SetCols(int cols);

  // Основные операции с матрицами
  bool EqMatrix(const S21Matrix& other) const;
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose() const;
  S21Matrix CalcComplements() const;
  double Determinant() const;
  S21Matrix InverseMatrix() const;
  S21Matrix GetMinor(int row, int col) const;

  // Перегруженные операторы
  S21Matrix operator+(const S21Matrix& other) const;
  S21Matrix operator-(const S21Matrix& other) const;
  S21Matrix operator*(const S21Matrix& other) const;
  S21Matrix operator*(const double num) const;
  friend S21Matrix operator*(const double num, const S21Matrix& matrix);
  bool operator==(const S21Matrix& other) const;
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix& operator=(S21Matrix&& other) noexcept;
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix& operator*=(const S21Matrix& other);
  S21Matrix& operator*=(const double num);
  double& operator()(int i, int j);
  const double& operator()(int i, int j) const;
};

#endif  // S21_MATRIX_OOP_H_