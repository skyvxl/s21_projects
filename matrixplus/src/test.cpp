#include <gtest/gtest.h>

#include "s21_matrix_oop.h"

TEST(CreateMatrix1, StandartConstructor) {
  S21Matrix matrix_a;
  ASSERT_TRUE(matrix_a.GetCols() == 0);
  ASSERT_TRUE(matrix_a.GetRows() == 0);
}

TEST(CreateMatrix2, ParamConstr) { ASSERT_NO_THROW(S21Matrix matrix(2, 2)); }

TEST(CreateMatrix3, ParamConstWithWrongSizes) {
  EXPECT_THROW(S21Matrix matrix(-1, 2), std::length_error);
}

TEST(CreateMatrix4, MoveConstr) {
  S21Matrix matrix_a(2, 2);
  ASSERT_NO_THROW(S21Matrix matrix_b = std::move(matrix_a));
}

TEST(CreateMatrix5, CopyConstr) {
  S21Matrix matrix_a(2, 2);
  ASSERT_NO_THROW(S21Matrix matrix_b(matrix_a));
}

TEST(OperatorParentheses3, MatrixsetterWrongIndexes) {
  S21Matrix matrix_a(2, 2);
  EXPECT_THROW(matrix_a(3, 0) = 1, std::logic_error);
}

TEST(eqMatrix3, NonEqMatrix) {
  S21Matrix matrix_a(1, 3);
  matrix_a(0, 0) = 1;
  matrix_a(0, 1) = 2;
  matrix_a(0, 2) = 3;
  S21Matrix matrix_b(2, 2);
  matrix_b(0, 0) = 1;
  matrix_b(0, 1) = 2;
  matrix_b(1, 0) = 3;
  matrix_b(1, 1) = 4;
  ASSERT_FALSE(matrix_a == matrix_b);
}

TEST(SumMatrix2, SumNonEqMatrix) {
  S21Matrix matrix_a(1, 2);
  S21Matrix matrix_b(2, 2);

  matrix_a(0, 0) = 1;
  matrix_a(0, 1) = 2;

  matrix_b(0, 0) = 1;
  matrix_b(0, 1) = 2;
  matrix_b(1, 0) = 3;
  matrix_b(1, 1) = 4;

  EXPECT_THROW(matrix_a.SumMatrix(matrix_b), std::logic_error);
}

TEST(SubMatrix2, SubNonEqMatrix) {
  S21Matrix matrix_a(1, 2);
  S21Matrix matrix_b(2, 2);

  matrix_a(0, 0) = 3;
  matrix_a(0, 1) = 2;

  matrix_b(0, 0) = -7;
  matrix_b(0, 1) = 0;
  matrix_b(1, 0) = -3;
  matrix_b(1, 1) = 2;

  EXPECT_THROW(matrix_a.SubMatrix(matrix_b), std::logic_error);
}

TEST(MulMatrix2, MulNonEqMatrix) {
  S21Matrix matrix_a(2, 1);
  S21Matrix matrix_b(2, 2);

  matrix_a(0, 0) = 3;
  matrix_a(1, 0) = -6.6;

  matrix_b(0, 0) = -7;
  matrix_b(0, 1) = 0;
  matrix_b(1, 0) = -3.5;
  matrix_b(1, 1) = 2;

  EXPECT_THROW(matrix_a.MulMatrix(matrix_b), std::logic_error);
}

TEST(OperatorSumMatrix2, OperatorSumWithNoEqSizesMatrix) {
  S21Matrix matrix_a(1, 2);
  S21Matrix matrix_b(2, 2);

  matrix_a(0, 0) = 1;
  matrix_a(0, 1) = 2;

  matrix_b(0, 0) = 1;
  matrix_b(0, 1) = 2;
  matrix_b(1, 0) = 3;
  matrix_b(1, 1) = 4;

  EXPECT_THROW(matrix_a += matrix_b, std::logic_error);
}

TEST(OperatorSubMatrix2, False) {
  S21Matrix matrix_a(1, 2);
  S21Matrix matrix_b(2, 2);

  matrix_a(0, 0) = 3;
  matrix_a(0, 1) = 2;

  matrix_b(0, 0) = -7;
  matrix_b(0, 1) = 0;
  matrix_b(1, 0) = -3;
  matrix_b(1, 1) = 2;

  EXPECT_THROW(matrix_a -= matrix_b, std::logic_error);
}

TEST(OperatorMulMatrix2, False) {
  S21Matrix matrix_a(2, 1);
  S21Matrix matrix_b(2, 2);

  matrix_a(0, 0) = 3;
  matrix_a(1, 0) = -6.6;

  matrix_b(0, 0) = -7;
  matrix_b(0, 1) = 0;
  matrix_b(1, 0) = -3.5;
  matrix_b(1, 1) = 2;

  EXPECT_THROW(matrix_a *= matrix_b, std::logic_error);
}

TEST(OperatorEqal, Invalid) {
  S21Matrix matrix_a(2, 1);
  S21Matrix matrix_b;

  EXPECT_THROW(matrix_a = matrix_b, std::logic_error);
}

TEST(invalidmatrix, False) {
  S21Matrix matrix_a(2, 1);
  S21Matrix matrix_b;

  matrix_a(0, 0) = 3;
  matrix_a(1, 0) = -6.6;

  EXPECT_THROW(matrix_b *= matrix_a, std::logic_error);
  EXPECT_THROW(matrix_b.EqMatrix(matrix_a), std::logic_error);
  EXPECT_THROW(matrix_b.Transpose(), std::logic_error);
  EXPECT_THROW(matrix_b.InverseMatrix(), std::logic_error);
  EXPECT_THROW(matrix_b.Determinant(), std::logic_error);
  EXPECT_THROW(matrix_b.CalcComplements(), std::logic_error);
  EXPECT_THROW(matrix_b *= 2, std::logic_error);
  EXPECT_THROW(matrix_b += matrix_a, std::logic_error);
  EXPECT_THROW(matrix_b -= matrix_a, std::logic_error);
}

TEST(Inverse1, False) {
  S21Matrix matrix_a(4, 3);

  matrix_a(0, 0) = 1;
  matrix_a(0, 1) = 2;
  matrix_a(0, 2) = 3;
  matrix_a(1, 0) = 4;
  matrix_a(1, 1) = 5;
  matrix_a(1, 2) = 6;
  matrix_a(2, 0) = 7;
  matrix_a(2, 1) = 8;
  matrix_a(2, 2) = 9;
  matrix_a(3, 0) = 10;
  matrix_a(3, 1) = 11;
  matrix_a(3, 2) = 12;

  EXPECT_THROW(matrix_a.InverseMatrix(), std::logic_error);
}

TEST(Det52, False) {
  S21Matrix matrix_a(4, 3);

  matrix_a(0, 0) = 1;
  matrix_a(0, 1) = 2;
  matrix_a(0, 2) = 3;
  matrix_a(1, 0) = 4;
  matrix_a(1, 1) = 5;
  matrix_a(1, 2) = 6;
  matrix_a(2, 0) = 7;
  matrix_a(2, 1) = 8;
  matrix_a(2, 2) = 9;
  matrix_a(3, 0) = 10;
  matrix_a(3, 1) = 11;
  matrix_a(3, 2) = 12;

  EXPECT_THROW(matrix_a.Determinant(), std::logic_error);
}

TEST(calc52, False) {
  S21Matrix matrix_a(4, 3);

  matrix_a(0, 0) = 1;
  matrix_a(0, 1) = 2;
  matrix_a(0, 2) = 3;
  matrix_a(1, 0) = 4;
  matrix_a(1, 1) = 5;
  matrix_a(1, 2) = 6;
  matrix_a(2, 0) = 7;
  matrix_a(2, 1) = 8;
  matrix_a(2, 2) = 9;
  matrix_a(3, 0) = 10;
  matrix_a(3, 1) = 11;
  matrix_a(3, 2) = 12;

  EXPECT_THROW(matrix_a.CalcComplements(), std::logic_error);
}

TEST(Inverse3, False) {
  S21Matrix matrix_a(3, 3);
  matrix_a(0, 0) = 1;
  matrix_a(0, 1) = 2;
  matrix_a(0, 2) = 3;
  matrix_a(1, 0) = 4;
  matrix_a(1, 1) = 5;
  matrix_a(1, 2) = 6;
  matrix_a(2, 0) = 7;
  matrix_a(2, 1) = 8;
  matrix_a(2, 2) = 9;

  EXPECT_THROW(matrix_a.InverseMatrix(), std::logic_error);
}

TEST(Constructor, Default) {
  auto test = S21Matrix();
  EXPECT_EQ(test.GetRows(), 0);
  EXPECT_EQ(test.GetCols(), 0);
}

TEST(Constructor, By2Args) {
  auto test = S21Matrix(3, 3);
  EXPECT_EQ(test.GetRows(), 3);
  EXPECT_EQ(test.GetCols(), 3);
}

TEST(Constructor, Copy) {
  auto test = S21Matrix(3, 3);
  test(0, 0) = 1;
  auto temp = S21Matrix(test);
  EXPECT_EQ(test, temp);
}

TEST(Constructor, Move) {
  auto test1 = S21Matrix(3, 3);
  test1(1, 1) = 5;
  auto test2 = S21Matrix(test1);
  auto test3 = S21Matrix(std::move(test1));
  EXPECT_EQ(test2, test3);
}

TEST(SimpleMathOperations, EqMatrix) {
  auto test1 = S21Matrix(3, 3);
  test1(1, 2) = 5;
  auto test2 = test1;
  EXPECT_TRUE(test1.EqMatrix(test2));
  test2(1, 1) = 1;
  EXPECT_FALSE(test1.EqMatrix(test2));
  auto test3 = S21Matrix(5, 5);
  EXPECT_FALSE(test1.EqMatrix(test3));
}

TEST(SimpleMathOperations, SumMatrix) {
  auto test1 = S21Matrix(1, 1);
  test1(0, 0) = 1;
  auto test2 = test1;
  test1.SumMatrix(test2);
  EXPECT_EQ(test1(0, 0), 2);
}

TEST(SimpleMathOperations, SubMatrix) {
  auto test1 = S21Matrix(1, 1);
  test1(0, 0) = 1;
  auto test2 = test1;
  test1.SubMatrix(test2);
  EXPECT_EQ(test1(0, 0), 0);
}

TEST(SimpleMathOperations, SubMatrix1) {
  auto test1 = S21Matrix(1, 1);
  test1(0, 0) = 1;
  auto test2 = test1;
  test1 = test1 - test2;
  EXPECT_EQ(test1(0, 0), 0);
}

TEST(SimpleMathOperations, SumMatrix1) {
  auto test1 = S21Matrix(2, 2);
  auto test_res = S21Matrix(2, 2);
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      test1(i, j) = 1;
      test_res(i, j) = 2;
    }
  }
  auto test2 = test1;
  test1 = (test1 + test2);
  EXPECT_EQ(test1, test_res);
}

TEST(SimpleMathOperations, MulMatrix2) {
  auto test1 = S21Matrix(1, 1);
  test1(0, 0) = 1;
  auto test2 = test1;
  test1 = (test1 * 0);
  EXPECT_EQ(test1(0, 0), 0);
}

TEST(SimpleMathOperations, MulNumber) {
  auto test1 = S21Matrix(1, 1);
  test1(0, 0) = 5;
  test1.MulNumber(3);
  EXPECT_EQ(test1(0, 0), 15);
}

TEST(SimpleMathOperations, MulMatrix) {
  auto test1 = S21Matrix(3, 3);
  for (int i = 0; i < test1.GetRows(); i++) {
    for (int j = 0; j < test1.GetCols(); j++) {
      test1(i, j) = test1.GetRows() * i + j + 1;
    }
  }
  auto test2 = test1;
  test1.MulMatrix(test2);
  test2(0, 0) = 30;
  test2(0, 1) = 36;
  test2(0, 2) = 42;
  test2(1, 0) = 66;
  test2(1, 1) = 81;
  test2(1, 2) = 96;
  test2(2, 0) = 102;
  test2(2, 1) = 126;
  test2(2, 2) = 150;
  EXPECT_EQ(test1, test2);
}

TEST(SimpleMathOperations, MulMatrix1) {
  auto test1 = S21Matrix(3, 3);
  for (int i = 0; i < test1.GetRows(); i++) {
    for (int j = 0; j < test1.GetCols(); j++) {
      test1(i, j) = test1.GetRows() * i + j + 1;
    }
  }
  auto test2 = test1;
  test1 *= test2;

  test2(0, 0) = 30;
  test2(0, 1) = 36;
  test2(0, 2) = 42;
  test2(1, 0) = 66;
  test2(1, 1) = 81;
  test2(1, 2) = 96;
  test2(2, 0) = 102;
  test2(2, 1) = 126;
  test2(2, 2) = 150;
  EXPECT_EQ(test1, test2);
}

TEST(LinearOperations, Transpose) {
  auto test1 = S21Matrix(3, 3);
  for (int i = 0; i < test1.GetRows(); i++) {
    for (int j = 0; j < test1.GetCols(); j++) {
      test1(i, j) = test1.GetRows() * i + j + 1;
    }
  }
  test1 = test1.Transpose();
  auto test2 = S21Matrix(3, 3);
  test2(0, 0) = 1;
  test2(0, 1) = 4;
  test2(0, 2) = 7;
  test2(1, 0) = 2;
  test2(1, 1) = 5;
  test2(1, 2) = 8;
  test2(2, 0) = 3;
  test2(2, 1) = 6;
  test2(2, 2) = 9;
  EXPECT_EQ(test1, test2);
}

TEST(LinearOperations, CalcComplements) {
  auto test1 = S21Matrix(3, 3);
  test1(0, 0) = 1;
  test1(0, 1) = 2;
  test1(0, 2) = 3;
  test1(1, 0) = 0;
  test1(1, 1) = 4;
  test1(1, 2) = 2;
  test1(2, 0) = 5;
  test1(2, 1) = 2;
  test1(2, 2) = 1;
  test1 = test1.CalcComplements();
  auto test2 = S21Matrix(3, 3);
  test2(0, 0) = 0;
  test2(0, 1) = 10;
  test2(0, 2) = -20;
  test2(1, 0) = 4;
  test2(1, 1) = -14;
  test2(1, 2) = 8;
  test2(2, 0) = -8;
  test2(2, 1) = -2;
  test2(2, 2) = 4;
  EXPECT_EQ(test1, test2);
}

TEST(LinearOperations, CalcComplements1) {
  auto test1 = S21Matrix(2, 3);
  test1(0, 0) = 1;
  test1(0, 1) = 2;
  test1(0, 2) = 3;
  test1(1, 0) = 0;
  test1(1, 1) = 4;
  test1(1, 2) = 2;
  EXPECT_THROW(test1.CalcComplements(), std::logic_error);
}

TEST(LinearOperations, Determinant) {
  auto test1 = S21Matrix(3, 3);
  test1(0, 0) = 1;
  test1(0, 1) = 2;
  test1(0, 2) = 3;
  test1(1, 0) = 0;
  test1(1, 1) = 4;
  test1(1, 2) = 2;
  test1(2, 0) = 5;
  test1(2, 1) = 2;
  test1(2, 2) = 1;
  EXPECT_DOUBLE_EQ(test1.Determinant(), -40);
}

TEST(LinearOperations, Determinant1) {
  auto test1 = S21Matrix(1, 1);
  test1(0, 0) = -40;
  EXPECT_DOUBLE_EQ(test1.Determinant(), -40);
}

TEST(LinearOperations, InverseMatrix) {
  auto test1 = S21Matrix(3, 3);
  test1(0, 0) = 2;
  test1(0, 1) = 5;
  test1(0, 2) = 7;
  test1(1, 0) = 6;
  test1(1, 1) = 3;
  test1(1, 2) = 4;
  test1(2, 0) = 5;
  test1(2, 1) = -2;
  test1(2, 2) = -3;
  test1 = test1.InverseMatrix();
  auto test2 = S21Matrix(3, 3);
  test2(0, 0) = 1;
  test2(0, 1) = -1;
  test2(0, 2) = 1;
  test2(1, 0) = -38;
  test2(1, 1) = 41;
  test2(1, 2) = -34;
  test2(2, 0) = 27;
  test2(2, 1) = -29;
  test2(2, 2) = 24;
  EXPECT_EQ(test1, test2);
}

TEST(Technical, GetRows) {
  auto test1 = S21Matrix();
  EXPECT_EQ(test1.GetRows(), 0);
  auto test2 = S21Matrix(2, 2);
  EXPECT_EQ(test2.GetRows(), 2);
}

TEST(Technical, GetCols) {
  auto test1 = S21Matrix();
  EXPECT_EQ(test1.GetCols(), 0);
  auto test2 = S21Matrix(2, 2);
  EXPECT_EQ(test2.GetCols(), 2);
}

TEST(Technical, SetRows) {
  auto test1 = S21Matrix(1, 1);
  test1.SetRows(5);
  EXPECT_EQ(test1.GetRows(), 5);
}

TEST(Technical, SetRows1) {
  auto test1 = S21Matrix(1, 1);
  EXPECT_THROW(test1.SetRows(-1), std::length_error);
}

TEST(Technical, SetCols) {
  auto test1 = S21Matrix(1, 1);
  test1.SetCols(5);
  EXPECT_EQ(test1.GetCols(), 5);
}

TEST(Technical, SetCols1) {
  auto test1 = S21Matrix(1, 1);
  EXPECT_THROW(test1.SetCols(0), std::length_error);
}

TEST(GetMinorTest, ThrowsForSmallMatrix) {
  S21Matrix matrix(1, 1);
  EXPECT_THROW(matrix.GetMinor(0, 0), std::logic_error);
}

TEST(GetMinorTest, ThrowsForOutOfRange) {
  S21Matrix matrix(3, 3);
  EXPECT_THROW(matrix.GetMinor(3, 0), std::out_of_range);
  EXPECT_THROW(matrix.GetMinor(0, 3), std::out_of_range);
  EXPECT_THROW(matrix.GetMinor(-1, 0), std::out_of_range);
}

TEST(CalcComplementsTest, For1x1Matrix) {
  S21Matrix matrix(1, 1);
  matrix(0, 0) = 5.0;
  S21Matrix result = matrix.CalcComplements();
  EXPECT_DOUBLE_EQ(result(0, 0), 1.0);
}

TEST(InverseMatrixTest, For1x1Matrix) {
  S21Matrix matrix(1, 1);
  matrix(0, 0) = 5.0;
  S21Matrix result = matrix.InverseMatrix();
  EXPECT_DOUBLE_EQ(result(0, 0), 0.2);  // 1/5 = 0.2
}

TEST(OperatorTest, MultiplyMatrix) {
  S21Matrix matrix1(2, 3);
  matrix1(0, 0) = 1;
  matrix1(0, 1) = 2;
  matrix1(0, 2) = 3;
  matrix1(1, 0) = 4;
  matrix1(1, 1) = 5;
  matrix1(1, 2) = 6;

  S21Matrix matrix2(3, 2);
  matrix2(0, 0) = 7;
  matrix2(0, 1) = 8;
  matrix2(1, 0) = 9;
  matrix2(1, 1) = 10;
  matrix2(2, 0) = 11;
  matrix2(2, 1) = 12;

  S21Matrix result = matrix1 * matrix2;
  EXPECT_EQ(result.GetRows(), 2);
  EXPECT_EQ(result.GetCols(), 2);
  EXPECT_DOUBLE_EQ(result(0, 0), 58);
  EXPECT_DOUBLE_EQ(result(0, 1), 64);
  EXPECT_DOUBLE_EQ(result(1, 0), 139);
  EXPECT_DOUBLE_EQ(result(1, 1), 154);
}

TEST(OperatorTest, MultiplyScalarFromLeft) {
  S21Matrix matrix(2, 2);
  matrix(0, 0) = 1;
  matrix(0, 1) = 2;
  matrix(1, 0) = 3;
  matrix(1, 1) = 4;

  S21Matrix result = 2.5 * matrix;
  EXPECT_DOUBLE_EQ(result(0, 0), 2.5);
  EXPECT_DOUBLE_EQ(result(0, 1), 5.0);
  EXPECT_DOUBLE_EQ(result(1, 0), 7.5);
  EXPECT_DOUBLE_EQ(result(1, 1), 10.0);
}

TEST(OperatorTest, SelfAssignment) {
  S21Matrix matrix(2, 2);
  matrix(0, 0) = 1;
  matrix(0, 1) = 2;
  matrix(1, 0) = 3;
  matrix(1, 1) = 4;

  matrix = matrix;
  EXPECT_EQ(matrix.GetRows(), 2);
  EXPECT_EQ(matrix.GetCols(), 2);
  EXPECT_DOUBLE_EQ(matrix(0, 0), 1);
  EXPECT_DOUBLE_EQ(matrix(0, 1), 2);
  EXPECT_DOUBLE_EQ(matrix(1, 0), 3);
  EXPECT_DOUBLE_EQ(matrix(1, 1), 4);
}

TEST(OperatorTest, AssignmentWithTemporary) {
  S21Matrix matrix1(2, 2);
  matrix1(0, 0) = 1;
  matrix1(0, 1) = 2;
  matrix1(1, 0) = 3;
  matrix1(1, 1) = 4;

  S21Matrix matrix2(3, 3);
  matrix2 = matrix1;

  EXPECT_EQ(matrix2.GetRows(), 2);
  EXPECT_EQ(matrix2.GetCols(), 2);
  EXPECT_DOUBLE_EQ(matrix2(0, 0), 1);
  EXPECT_DOUBLE_EQ(matrix2(0, 1), 2);
  EXPECT_DOUBLE_EQ(matrix2(1, 0), 3);
  EXPECT_DOUBLE_EQ(matrix2(1, 1), 4);
}

TEST(OperatorTest, PlusEqualsReturnValue) {
  S21Matrix matrix1(2, 2);
  matrix1(0, 0) = 1;
  matrix1(0, 1) = 2;
  matrix1(1, 0) = 3;
  matrix1(1, 1) = 4;

  S21Matrix matrix2(2, 2);
  matrix2(0, 0) = 5;
  matrix2(0, 1) = 6;
  matrix2(1, 0) = 7;
  matrix2(1, 1) = 8;

  S21Matrix& ref = (matrix1 += matrix2);
  EXPECT_EQ(&ref,
            &matrix1);  // Проверяем, что возвращается ссылка на тот же объект
  EXPECT_DOUBLE_EQ(matrix1(0, 0), 6);
  EXPECT_DOUBLE_EQ(matrix1(0, 1), 8);
  EXPECT_DOUBLE_EQ(matrix1(1, 0), 10);
  EXPECT_DOUBLE_EQ(matrix1(1, 1), 12);
}

TEST(OperatorTest, MinusEqualsReturnValue) {
  S21Matrix matrix1(2, 2);
  matrix1(0, 0) = 6;
  matrix1(0, 1) = 8;
  matrix1(1, 0) = 10;
  matrix1(1, 1) = 12;

  S21Matrix matrix2(2, 2);
  matrix2(0, 0) = 1;
  matrix2(0, 1) = 2;
  matrix2(1, 0) = 3;
  matrix2(1, 1) = 4;

  S21Matrix& ref = (matrix1 -= matrix2);
  EXPECT_EQ(&ref,
            &matrix1);  // Проверяем, что возвращается ссылка на тот же объект
  EXPECT_DOUBLE_EQ(matrix1(0, 0), 5);
  EXPECT_DOUBLE_EQ(matrix1(0, 1), 6);
  EXPECT_DOUBLE_EQ(matrix1(1, 0), 7);
  EXPECT_DOUBLE_EQ(matrix1(1, 1), 8);
}

TEST(OperatorTest, MultiplyEqualsScalarReturnValue) {
  S21Matrix matrix(2, 2);
  matrix(0, 0) = 1;
  matrix(0, 1) = 2;
  matrix(1, 0) = 3;
  matrix(1, 1) = 4;

  S21Matrix& ref = (matrix *= 2.5);
  EXPECT_EQ(&ref,
            &matrix);  // Проверяем, что возвращается ссылка на тот же объект
  EXPECT_DOUBLE_EQ(matrix(0, 0), 2.5);
  EXPECT_DOUBLE_EQ(matrix(0, 1), 5.0);
  EXPECT_DOUBLE_EQ(matrix(1, 0), 7.5);
  EXPECT_DOUBLE_EQ(matrix(1, 1), 10.0);
}

TEST(OperatorTest, ParenthesesInvalidMatrix) {
  S21Matrix matrix;  // Создаем пустую матрицу
  EXPECT_THROW(matrix(0, 0), std::logic_error);
}

TEST(OperatorTest, ParenthesesConstVersion) {
  S21Matrix matrix(2, 2);
  matrix(0, 0) = 1;
  matrix(0, 1) = 2;
  matrix(1, 0) = 3;
  matrix(1, 1) = 4;

  const S21Matrix const_matrix = matrix;
  EXPECT_DOUBLE_EQ(const_matrix(0, 0), 1);
  EXPECT_DOUBLE_EQ(const_matrix(0, 1), 2);
  EXPECT_DOUBLE_EQ(const_matrix(1, 0), 3);
  EXPECT_DOUBLE_EQ(const_matrix(1, 1), 4);

  EXPECT_THROW(const_matrix(2, 0), std::logic_error);
}

TEST(GetMinorTest, SuccessfulCase) {
  S21Matrix matrix(3, 3);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      matrix(i, j) = i * 3 + j + 1;
    }
  }

  S21Matrix minor = matrix.GetMinor(1, 1);
  EXPECT_EQ(minor.GetRows(), 2);
  EXPECT_EQ(minor.GetCols(), 2);
  EXPECT_DOUBLE_EQ(minor(0, 0), 1);
  EXPECT_DOUBLE_EQ(minor(0, 1), 3);
  EXPECT_DOUBLE_EQ(minor(1, 0), 7);
  EXPECT_DOUBLE_EQ(minor(1, 1), 9);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}