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

template <typename T>
class QRMatrix {
public:
    QRMatrix(size_t _w, size_t _h): matrix(_w*_h), w(_w), h(_h) {}
    QRMatrix(std::initializer_list<std::initializer_list<T>> initList)
    : QRMatrix(initList.size(), initList.begin()->size()) {
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
    T* operator[](size_t index);
    const T* operator[](size_t index) const;

    size_t width() const {return w;}
    size_t height() const {return h;}
    size_t rowCnt() const {return h;}
    size_t columnCnt() const {return w;}
    size_t getSize() const {return w*h;}

    T* getPureArray() const {return matrix.getPureArray();}

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
