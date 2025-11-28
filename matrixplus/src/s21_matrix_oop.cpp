#include "s21_matrix_oop.h"

void S21Matrix::CheckMatrixForValidity() const {
  if (rows_ <= 0 || cols_ <= 0 || matrix_ == nullptr) {
    throw std::logic_error("Matrix is invalid");
  }
}

// Базовый конструктор
S21Matrix::S21Matrix() : rows_(0), cols_(0), matrix_(nullptr) {
  // По умолчанию матрица 0x0
}

// Конструктор с параметрами
S21Matrix::S21Matrix(int rows, int cols)
    : rows_(rows), cols_(cols), matrix_(nullptr) {
  if (rows <= 0 || cols <= 0) {
    throw std::length_error("Matrix dimensions must be positive");
  }
  matrix_ = new double*[rows_];
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = new double[cols_]();
  }
}

// Конструктор копирования
S21Matrix::S21Matrix(const S21Matrix& other)
    : rows_(other.rows_), cols_(other.cols_), matrix_(nullptr) {
  if (other.rows_ > 0 && other.cols_ > 0) {
    matrix_ = new double*[rows_];
    for (int i = 0; i < rows_; i++) {
      matrix_[i] = new double[cols_];
      for (int j = 0; j < cols_; j++) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
  }
}

// Конструктор переноса
S21Matrix::S21Matrix(S21Matrix&& other) noexcept
    : rows_(0), cols_(0), matrix_(nullptr) {
  std::swap(rows_, other.rows_);
  std::swap(cols_, other.cols_);
  std::swap(matrix_, other.matrix_);
}

// Деструктор
S21Matrix::~S21Matrix() {
  if (matrix_) {
    for (int i = 0; i < rows_; i++) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
    matrix_ = nullptr;
  }
  rows_ = 0;
  cols_ = 0;
}

// Геттеры и сеттеры
int S21Matrix::GetRows() const { return rows_; }

int S21Matrix::GetCols() const { return cols_; }

void S21Matrix::SetRows(int rows) {
  if (rows <= 0) {
    throw std::length_error("Number of rows must be positive");
  }

  CheckMatrixForValidity();

  double** new_matrix = new double*[rows];
  for (int i = 0; i < rows; i++) {
    new_matrix[i] = new double[cols_]();
    // Копируем существующие значения
    if (i < rows_) {
      for (int j = 0; j < cols_; j++) {
        new_matrix[i][j] = matrix_[i][j];
      }
    }
  }

  // Освобождаем старую память
  for (int i = 0; i < rows_; i++) {
    delete[] matrix_[i];
  }
  delete[] matrix_;

  matrix_ = new_matrix;
  rows_ = rows;
}

void S21Matrix::SetCols(int cols) {
  if (cols <= 0) {
    throw std::length_error("Number of columns must be positive");
  }

  CheckMatrixForValidity();

  double** new_matrix = new double*[rows_];
  for (int i = 0; i < rows_; i++) {
    new_matrix[i] = new double[cols]();
    // Копируем существующие значения
    for (int j = 0; j < cols; j++) {
      if (j < cols_) {
        new_matrix[i][j] = matrix_[i][j];
      }
    }
    delete[] matrix_[i];
  }

  delete[] matrix_;

  matrix_ = new_matrix;
  cols_ = cols;
}

// Сравнение матриц
bool S21Matrix::EqMatrix(const S21Matrix& other) const {
  CheckMatrixForValidity();

  if (rows_ != other.rows_ || cols_ != other.cols_) {
    return false;
  }

  const double epsilon = 1e-7;
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      if (fabs(matrix_[i][j] - other.matrix_[i][j]) > epsilon) {
        return false;
      }
    }
  }

  return true;
}

// Сложение матриц
void S21Matrix::SumMatrix(const S21Matrix& other) {
  CheckMatrixForValidity();

  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::logic_error("Matrix dimensions do not match for addition");
  }

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] += other.matrix_[i][j];
    }
  }
}

// Вычитание матриц
void S21Matrix::SubMatrix(const S21Matrix& other) {
  CheckMatrixForValidity();

  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::logic_error("Matrix dimensions do not match for subtraction");
  }

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

// Умножение на число
void S21Matrix::MulNumber(const double num) {
  CheckMatrixForValidity();

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] *= num;
    }
  }
}

// Умножение матриц
void S21Matrix::MulMatrix(const S21Matrix& other) {
  CheckMatrixForValidity();

  if (cols_ != other.rows_) {
    throw std::logic_error(
        "Number of columns of first matrix must equal number of rows of second "
        "matrix");
  }

  S21Matrix result(rows_, other.cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      for (int k = 0; k < cols_; k++) {
        result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }

  *this = std::move(result);
}

// Транспонирование матрицы
S21Matrix S21Matrix::Transpose() const {
  CheckMatrixForValidity();

  S21Matrix result(cols_, rows_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      result.matrix_[j][i] = matrix_[i][j];
    }
  }
  return result;
}

// Получение минора матрицы, исключая указанную строку и столбец
S21Matrix S21Matrix::GetMinor(int row, int col) const {
  CheckMatrixForValidity();

  if (rows_ <= 1 || cols_ <= 1) {
    throw std::logic_error("Cannot get minor of 1x1 or smaller matrix");
  }
  if (row < 0 || row >= rows_ || col < 0 || col >= cols_) {
    throw std::out_of_range("Row or column index out of range");
  }

  S21Matrix result(rows_ - 1, cols_ - 1);
  int r_new = 0;
  for (int i = 0; i < rows_; i++) {
    if (i == row) continue;

    int c_new = 0;
    for (int j = 0; j < cols_; j++) {
      if (j == col) continue;
      result.matrix_[r_new][c_new] = matrix_[i][j];
      c_new++;
    }
    r_new++;
  }

  return result;
}

// Вычисление матрицы алгебраических дополнений
S21Matrix S21Matrix::CalcComplements() const {
  CheckMatrixForValidity();

  if (rows_ != cols_) {
    throw std::logic_error("Matrix must be square for calculating complements");
  }

  S21Matrix result(rows_, cols_);
  if (rows_ == 1) {
    result.matrix_[0][0] = 1;
    return result;
  }

  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      S21Matrix minor = GetMinor(i, j);
      double det = minor.Determinant();
      result.matrix_[i][j] = det * pow(-1, i + j);
    }
  }

  return result;
}

// Вычисление определителя матрицы
double S21Matrix::Determinant() const {
  CheckMatrixForValidity();

  if (rows_ != cols_) {
    throw std::logic_error("Matrix must be square for calculating determinant");
  }

  if (rows_ == 1) {
    return matrix_[0][0];
  }

  if (rows_ == 2) {
    return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
  }

  double result = 0;
  for (int j = 0; j < cols_; j++) {
    S21Matrix minor = GetMinor(0, j);
    result += matrix_[0][j] * pow(-1, j) * minor.Determinant();
  }

  return result;
}

// Вычисление обратной матрицы
S21Matrix S21Matrix::InverseMatrix() const {
  CheckMatrixForValidity();

  if (rows_ != cols_) {
    throw std::logic_error("Matrix must be square for calculating inverse");
  }

  double det = Determinant();
  if (fabs(det) < 1e-7) {
    throw std::logic_error("Matrix is singular, cannot calculate inverse");
  }

  if (rows_ == 1) {
    S21Matrix result(1, 1);
    result.matrix_[0][0] = 1.0 / matrix_[0][0];
    return result;
  }

  S21Matrix complements = CalcComplements();
  S21Matrix transposed = complements.Transpose();
  transposed.MulNumber(1.0 / det);

  return transposed;
}

// Перегруженные операторы
S21Matrix S21Matrix::operator+(const S21Matrix& other) const {
  CheckMatrixForValidity();

  S21Matrix result(*this);
  result.SumMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) const {
  CheckMatrixForValidity();

  S21Matrix result(*this);
  result.SubMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) const {
  CheckMatrixForValidity();

  S21Matrix result(*this);
  result.MulMatrix(other);
  return result;
}

S21Matrix S21Matrix::operator*(const double num) const {
  CheckMatrixForValidity();

  S21Matrix result(*this);
  result.MulNumber(num);
  return result;
}

S21Matrix operator*(const double num, const S21Matrix& matrix) {
  return matrix * num;
}

bool S21Matrix::operator==(const S21Matrix& other) const {
  return EqMatrix(other);
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  if (this == &other) {
    return *this;  // Самоприсваивание
  }

  // Проверяем, что other - валидная матрица
  if (other.rows_ <= 0 || other.cols_ <= 0 || other.matrix_ == nullptr) {
    throw std::logic_error("Cannot assign invalid matrix");
  }

  // Создаем временную копию и затем свопаем
  S21Matrix temp(other);
  std::swap(rows_, temp.rows_);
  std::swap(cols_, temp.cols_);
  std::swap(matrix_, temp.matrix_);

  return *this;
}

S21Matrix& S21Matrix::operator=(S21Matrix&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  // Освобождаем текущие ресурсы
  if (matrix_) {
    for (int i = 0; i < rows_; i++) {
      delete[] matrix_[i];
    }
    delete[] matrix_;
  }

  // Переносим ресурсы из other
  rows_ = other.rows_;
  cols_ = other.cols_;
  matrix_ = other.matrix_;

  // Обнуляем other
  other.rows_ = 0;
  other.cols_ = 0;
  other.matrix_ = nullptr;

  return *this;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  SumMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  SubMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  MulMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const double num) {
  CheckMatrixForValidity();
  MulNumber(num);
  return *this;
}

double& S21Matrix::operator()(int i, int j) {
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
    throw std::logic_error("Matrix indices out of range");
  }

  // Дополнительно проверяем на наличие матрицы
  if (matrix_ == nullptr) {
    throw std::logic_error("Matrix is invalid");
  }

  return matrix_[i][j];
}

const double& S21Matrix::operator()(int i, int j) const {
  if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
    throw std::logic_error("Matrix indices out of range");
  }

  // Дополнительно проверяем на наличие матрицы
  if (matrix_ == nullptr) {
    throw std::logic_error("Matrix is invalid");
  }

  return matrix_[i][j];
}