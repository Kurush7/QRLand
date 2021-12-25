//
// Created by kurush on 29.07.2020.
//

#ifndef BIG3DFLUFFY_QRMATRIX_H
#define BIG3DFLUFFY_QRMATRIX_H

#include "../QRVector/QRVector.h"
#include <initializer_list>
#include <cmath>
#include <iostream>
#include <cstdio>
#include <memory>
#include "../exceptions/VectorError/Errors.h"
#include "globalDefines.h"

const size_t DEFAULT_MATRIX_SIZE = 4;

template <typename T>
class QRMatrix {
public:
    QRMatrix(): QRMatrix(DEFAULT_MATRIX_SIZE, DEFAULT_MATRIX_SIZE) {}
    QRMatrix(size_t _w, size_t _h): matrix(_w*_h), w(_w), h(_h) {}
    QRMatrix(std::initializer_list<std::initializer_list<T>> initList)
    : QRMatrix(initList.begin()->size(), initList.size()) {
        std::initializer_list<T> row;
        w = initList.begin()->size();
        h = initList.size();
        size_t i = 0, j;
        for (auto &row : initList) {
            j = 0;
            for (auto &elem : row)
                matrix[i*w + j] = elem, j++;
            i++;
        }
    }
    void deepCopy(const QRMatrix& m) {
        w = m.width(), h = m.height();
        matrix.reserve(w*h);
        matrix.setSize(w*h);
        for (int i = 0; i < h; ++i)
            for (int j = 0; j < w; ++j)
                matrix[i*w+j] = m[i][j];
    }

    T* operator[](size_t index);
    const T* operator[](size_t index) const;

    size_t width() const {return w;}
    size_t height() const {return h;}
    size_t rowCnt() const {return h;}
    size_t columnCnt() const {return w;}
    size_t getSize() const {return w*h;}

    T* getPureArray() const {return matrix.getPureArray();}
    const QRVector<T>& getArray() const {return matrix;}

    void fill(const T &x) {
        for (size_t i = 0; i < w*h; ++i)
            matrix[i] = x;
    }

    void resize(size_t _w, size_t _h) {
        w = _w, h = _h;
        matrix.reserve(w*h);
        matrix.setSize(w*h);
    }

    QRVectorIterator<T> begin() const {return matrix.begin();}
    QRVectorIterator<T> end() const {return matrix.end();}

private:
    QRVector<T> matrix;
    size_t w, h;
};

template <typename T>
T* QRMatrix<T>::operator[](size_t index) { return &matrix[index*w];}

template <typename T>
const T* QRMatrix<T>::operator[](size_t index) const{ return &matrix[index*w];}

#endif //BIG3DFLUFFY_QRMATRIX_H
