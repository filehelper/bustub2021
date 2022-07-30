//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// p0_starter.h
//
// Identification: src/include/primer/p0_starter.h
//
// Copyright (c) 2015-2020, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include "common/exception.h"

namespace bustub {

using bustub::Exception;
using bustub::ExceptionType;
/**
 * The Matrix type defines a common
 * interface for matrix operations.
 */
template <typename T>
class Matrix {
 protected:
  /**
   * TODO(P0): Add implementation
   *
   * Construct a new Matrix instance.
   * @param rows The number of rows
   * @param cols The number of columns
   *
   */
  Matrix(int rows, int cols) : rows_{rows}, cols_{cols} {}

  /** The number of rows in the matrix */
  int rows_;
  /** The number of columns in the matrix */
  int cols_;

  /**
   * TODO(P0): Allocate the array in the constructor.
   * TODO(P0): Deallocate the array in the destructor.
   * A flattened array containing the elements of the matrix.
   */
  T *linear_;

 public:
  /** @return The number of rows in the matrix */
  virtual auto GetRowCount() const -> int = 0;

  /** @return The number of columns in the matrix */
  virtual auto GetColumnCount() const -> int = 0;

  /**
   * Get the (i,j)th matrix element.
   *
   * Throw OUT_OF_RANGE if either index is out of range.
   *
   * @param i The row index
   * @param j The column index
   * @return The (i,j)th matrix element
   * @throws OUT_OF_RANGE if either index is out of range
   */
  virtual auto GetElement(int i, int j) const -> T = 0;

  /**
   * Set the (i,j)th matrix element.
   *
   * Throw OUT_OF_RANGE if either index is out of range.
   *
   * @param i The row index
   * @param j The column index
   * @param val The value to insert
   * @throws OUT_OF_RANGE if either index is out of range
   */
  virtual void SetElement(int i, int j, T val) = 0;

  /**
   * Fill the elements of the matrix from `source`.
   *
   * Throw OUT_OF_RANGE in the event that `source`
   * does not contain the required number of elements.
   *
   * @param source The source container
   * @throws OUT_OF_RANGE if `source` is incorrect size
   */
  virtual void FillFrom(const std::vector<T> &source) = 0;

  /**
   * Destroy a matrix instance.
   * TODO(P0): Add implementation
   */
  virtual ~Matrix() = default;
};

/**
 * The RowMatrix type is a concrete matrix implementation.
 * It implements the interface defined by the Matrix type.
 */
template <typename T>
class RowMatrix : public Matrix<T> {
 public:
  /**
   * TODO(P0): Add implementation
   *
   * Construct a new RowMatrix instance.
   * @param rows The number of rows
   * @param cols The number of columns
   */
  RowMatrix(int rows, int cols) : Matrix<T>(rows, cols) {
    data_ = new T *[rows];
    for (int i = 0; i < this->rows_; ++i) {
      data_[i] = new T[this->cols_];
    }
  }

  /**
   * TODO(P0): Add implementation
   * @return The number of rows in the matrix
   */
  auto GetRowCount() const -> int override { return this->rows_; }

  /**
   * TODO(P0): Add implementation
   * @return The number of columns in the matrix
   */
  auto GetColumnCount() const -> int override { return this->cols_; }

  /**
   * TODO(P0): Add implementation
   *
   * Get the (i,j)th matrix element.
   *
   * Throw OUT_OF_RANGE if either index is out of range.
   *
   * @param i The row index
   * @param j The column index
   * @return The (i,j)th matrix element
   * @throws OUT_OF_RANGE if either index is out of range
   */
  auto GetElement(int i, int j) const -> T override {
    if (i < 0 || i >= this->rows_ || j < 0 || j >= this->cols_) {
      throw bustub::Exception(ExceptionType::OUT_OF_RANGE, "GetElement error!");
    }
    return data_[i][j];
  }

  /**
   * Set the (i,j)th matrix element.
   *
   * Throw OUT_OF_RANGE if either index is out of range.
   *
   * @param i The row index
   * @param j The column index
   * @param val The value to insert
   * @throws OUT_OF_RANGE if either index is out of range
   */
  void SetElement(int i, int j, T val) override {
    if (i < 0 || i >= this->rows_ || j < 0 || j >= this->cols_) {
      throw bustub::Exception(ExceptionType::OUT_OF_RANGE, "SetElement error!");
    }
    data_[i][j] = val;
  }

  /**
   * TODO(P0): Add implementation
   *
   * Fill the elements of the matrix from `source`.
   *
   * Throw OUT_OF_RANGE in the event that `source`
   * does not contain the required number of elements.
   *
   * @param source The source container
   * @throws OUT_OF_RANGE if `source` is incorrect size
   */
  void FillFrom(const std::vector<T> &source) override {
    if (static_cast<int>(source.size()) != (this->cols_ * this->rows_)) {
      throw bustub::Exception(ExceptionType::OUT_OF_RANGE, "FillFrom error!");
    }
    int pos = 0;
    for (int i = 0; i < this->rows_; ++i) {
      for (int j = 0; j < this->cols_; ++j) {
        data_[i][j] = source[pos];
        ++pos;
      }
    }
  }

  /**
   * TODO(P0): Add implementation
   *
   * Destroy a RowMatrix instance.
   */
  ~RowMatrix() override {
    for (int i = 0; i < this->rows_; ++i) {
      delete[] data_[i];
    }
    delete[] data_;
  }

 private:
  /**
   * A 2D array containing the elements of the matrix in row-major format.
   *
   * TODO(P0):
   * - Allocate the array of row pointers in the constructor.
   * - Use these pointers to point to corresponding elements of the `linear` array.
   * - Don't forget to deallocate the array in the destructor.
   */
  T **data_;
};

/**
 * The RowMatrixOperations class defines operations
 * that may be performed on instances of `RowMatrix`.
 */
template <typename T>
class RowMatrixOperations {
 public:
  /**
   * Compute (`matrixA` + `matrixB`) and return the result.
   * Return `nullptr` if dimensions mismatch for input matrices.
   * @param matrixA Input matrix
   * @param matrixB Input matrix
   * @return The result of matrix addition
   */
  static auto Add(const RowMatrix<T> *matrixA, const RowMatrix<T> *matrixB) -> std::unique_ptr<RowMatrix<T>> {
    int row = matrixA->GetRowCount();
    int col = matrixA->GetColumnCount();
    std::unique_ptr<RowMatrix<T>> ret = std::make_unique<RowMatrix<T>>(row, col);
    for (int i = 0; i < row; ++i) {
      for (int j = 0; j < col; ++j) {
        ret->SetElement(i, j, matrixA->GetElement(i, j) + matrixB->GetElement(i, j));
      }
    }
    return ret;
  }

  /**
   * Compute the matrix multiplication (`matrixA` * `matrixB` and return the result.
   * Return `nullptr` if dimensions mismatch for input matrices.
   * @param matrixA Input matrix
   * @param matrixB Input matrix
   * @return The result of matrix multiplication
   */
  static auto Multiply(const RowMatrix<T> *matrixA, const RowMatrix<T> *matrixB) -> std::unique_ptr<RowMatrix<T>> {
    if (matrixA->GetColumnCount() != matrixB->GetRowCount()) {
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    }
    std::unique_ptr<RowMatrix<T>> ret =
        std::make_unique<RowMatrix<T>>(matrixA->GetRowCount(), matrixB->GetColumnCount());
    for (int i = 0; i < matrixA->GetRowCount(); ++i) {
      for (int j = 0; j < matrixB->GetColumnCount(); ++j) {
        int tmp = 0;
        for (int k = 0; k < matrixA->GetColumnCount(); ++k) {
          tmp += matrixA->GetElement(i, k) * matrixB->GetElement(k, j);
        }
        ret->SetElement(i, j, tmp);
      }
    }
    return ret;
  }

  /**
   * Simplified General Matrix Multiply operation. Compute (`matrixA` * `matrixB` + `matrixC`).
   * Return `nullptr` if dimensions mismatch for input matrices.
   * @param matrixA Input matrix
   * @param matrixB Input matrix
   * @param matrixC Input matrix
   * @return The result of general matrix multiply
   */
  static auto GEMM(const RowMatrix<T> *matrixA, const RowMatrix<T> *matrixB, const RowMatrix<T> *matrixC)
      -> std::unique_ptr<RowMatrix<T>> {
    int arow = matrixA->GetRowCount();
    int acol = matrixA->GetColumnCount();
    int brow = matrixB->GetRowCount();
    int bcol = matrixB->GetColumnCount();
    int crow = matrixC->GetRowCount();
    int ccol = matrixC->GetColumnCount();

    if (acol != brow || arow != crow || bcol != ccol) {
      return std::unique_ptr<RowMatrix<T>>(nullptr);
    }

    std::unique_ptr<RowMatrix<T>> addsum = Multiply(matrixA, matrixB);
    return Add(addsum.get(), matrixC);
  }
};
}  // namespace bustub

// Autograder Score
// 100.0 / 100.0
// Passed Tests
//     test_Starter_Add (__main__.TestProject0) (20.0/20.0)
//     test_Starter_Elements (__main__.TestProject0) (10.0/10.0)
//     test_Starter_Gemm (__main__.TestProject0) (20.0/20.0)
//     test_Starter_Init (__main__.TestProject0) (5.0/5.0)
//     test_Starter_Multiply (__main__.TestProject0) (20.0/20.0)
//     test_Starter_SampleTest (__main__.TestProject0) (5.0/5.0)
//     test_build (__main__.TestProject0) (0.0/0.0)
//     test_memory_safety (__main__.TestProject0) (20.0/20.0)
