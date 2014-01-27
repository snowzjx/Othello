//
//  MatrixData.h
//  Othello
//
//  Created by Snow on 1/19/14.
//
//

#ifndef _MATRIX_DATA_H_
#define _MATRIX_DATA_H_

template<typename T>
class MatrixData final {
private:
    T* _data = nullptr;
    int _row = 0;
    int _column = 0;
    int _rowIndex = 0;
public:
    MatrixData(int row, int column) {
        this->_row = row;
        this->_column = column;
        if (this->_data != nullptr) {
            delete[] this->_data;
        }
        this->_data = new T[row * column];
        memset(this->_data, 0, sizeof(T) * row * column);
    }
    
    MatrixData(MatrixData<T>& matrixData) {
        this->_row = matrixData._row;
        this->_column = matrixData._column;
        this->_data = new T[_row * _column];
        memcpy(this->_data, matrixData._data, sizeof(T) * _row * _column);
    }
    
    void setRowIndex(int rowIndex) {
        this->_rowIndex = rowIndex;
    }
    
    T& operator[](int index) {
        return this->_data[_rowIndex * _column + index];
    }
    
    bool operator==(const MatrixData &matrixData) const {
        if (this->_row != matrixData._row || this->_column != matrixData._column) {
            return false;
        }
        for (int i = 0; i < _row * _column; i++) {
            if (this->_data[i] != matrixData._data[i]) {
                return false;
            }
        }
        return true;
    }
    
    ~MatrixData() {
        if (this->_data != nullptr) {
            delete[] this->_data;
        }
    }
};

#endif
