//
//  Matrix.h
//  Othello
//
//  Created by Snow on 1/19/14.
//
//

#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <memory>

#include "MatrixData.h"

template<typename T>
class Matrix final {
private:
    MatrixData<T> *_matrixData = nullptr;
    int _row = 0;
    int _column = 0;
public:
    Matrix(int row, int column) {
        this->_row = row;
        this->_column = column;
        this->_matrixData = new MatrixData<T>(row, column);
    }
    
    Matrix(Matrix<T>& matrix) {
        this->_row = matrix._row;
        this->_column = matrix._column;
        this->_matrixData = new MatrixData<T>(*matrix._matrixData);
    }
    
    int getRow() {
        return this->_row;
    }
    
    int getColumn() {
        return this->_column;
    }
    
    MatrixData<T>& operator[](int index) {
        this->_matrixData->setRowIndex(index);
        return *this->_matrixData;
    }
    
    bool operator==(const Matrix<T> &matrix) const {
        if (this->_row != matrix.row || this->_column != matrix.column) {
            return false;
        }
        return *this->_matrixData == *matrix.matrixData;
    }
    
    std::shared_ptr<Matrix<T>> clone() {
        std::shared_ptr<Matrix<T>> pMatrix(new Matrix<T>(*this));
        return pMatrix;
    }
    
    ~Matrix() {
        if (this->_matrixData != nullptr) {
            delete this->_matrixData;
        }
    }
};

#endif
