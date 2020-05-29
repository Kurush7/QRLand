//
// Created by Георгий Куликов on 27.04.2020.
//

#ifndef QRVECTOR_H
#define QRVECTOR_H


#include <initializer_list>
#include <cmath>
#include <iostream>
#include "VectorBase.h"
#include "VectorIterator/VectorIterator.h"

const double DEFAULT_SIZE = 8;

template <typename T>
class QRVector: public VectorBase {
private:
    std::shared_ptr<T[]> arr;
    void grow();
public:
    explicit QRVector(size_t = DEFAULT_SIZE);
    QRVector(size_t, T*);
    QRVector(std::initializer_list<T>);
    QRVector(const QRVector<T> &);
    QRVector(QRVector<T>&&);

    QRVector<T>& operator =(const QRVector<T>&);
    QRVector<T>& operator =(const QRVector<T>&&);
    QRVector<T>& operator =(std::initializer_list<T>);

    VectorIterator<T> begin() const;
    VectorIterator<T> end() const;

    T& operator [](int index);
    const T& operator [](size_t index) const;
    void push_back(const T& newValue);

    size_t len() const;

    bool operator ==(const QRVector<T>& vec) const;
    bool operator !=(const QRVector<T>& vec) const;
    bool isEqual(const QRVector<T>& vec) const;
    bool isNotEqual(const QRVector<T>& vec) const;
};

template<typename T>
std::ostream& operator <<(std::ostream &os, const QRVector<T> &vec);

template<typename T>
void QRVector<T>::grow() {
    try {
        max_size *= 2;
        auto new_arr = std::shared_ptr<T[]>(new T[max_size]);
        for (size_t i = 0; i < *size; ++i)
            new_arr[i] = arr[i];
        arr = new_arr;
    }
    catch (bad_alloc &exc) {
        throw ErrorAlloc(__FILE__, __LINE__, __TIME__, "can't allocate memory");
    }
}

template<typename T>
QRVector<T>::QRVector(size_t sz):VectorBase(sz) {
    try {
        max_size = DEFAULT_SIZE;
        while (max_size < sz)
            max_size *= 2;
        size = shared_ptr<size_t>(new size_t(0));
        this->arr = std::shared_ptr<T[]>(new T[max_size]);
    }
    catch (bad_alloc &exc) {
        throw ErrorAlloc(__FILE__, __LINE__, __TIME__, "can't allocate memory");
    }
}

template<typename T>
QRVector<T>::QRVector(size_t sz, T *vec): VectorBase(sz) {
    if (!vec)
        throw ErrorNullptr(__FILE__, __LINE__, __TIME__, "can't initialize vector with nullptr");
    try {
        for (size_t i = 0; i < sz; ++i) {
            arr[i] = vec[i];
        }
    }
    catch (bad_alloc &exc) {
        throw ErrorAlloc(__FILE__, __LINE__, __TIME__, "can't allocate memory");
    }
}


template<typename T>
QRVector<T>::QRVector(std::initializer_list<T> initList): QRVector(initList.size()) {
    size_t i = 0;
    for (auto &elem : initList)
        arr[i] = elem;
}

template<typename T>
QRVector<T>::QRVector(const QRVector<T> &vec) {
    while (*size < vec.len())
        grow();

    size = vec.size;
    for (size_t i = 0; i < vec.len(); ++i)
        arr[i] = vec[i];
}

template<typename T>
QRVector<T>::QRVector(QRVector<T> &&vec) {
    arr = vec.arr;
    size = vec.size;
    max_size = vec.max_size;
    vec.arr.reset();
    vec.size.reset();
}


template<typename T>
VectorIterator<T> QRVector<T>::begin() const {
    return VectorIterator<T>(arr, size);
}

template<typename T>
VectorIterator<T> QRVector<T>::end() const {
    return VectorIterator<T>(arr, size, *size);
}


template<typename T>
void QRVector<T>::push_back(const T &val) {
    if (*size >= max_size - 1) grow();
    arr[*size] = val;
    (*size)++;
}

template<typename T>
T& QRVector<T>::operator[](int index) {
    return this->getElem(index);
}

template<typename T>
const T& QRVector<T>::operator[](size_t index) const {
    return this->getElem(index);
}



template<typename T>
std::ostream& operator <<(std::ostream &os, const QRVector<T> &vec) {
    os << "<QRVector of " << vec.len() << ' ' << "elements>";
}



template<typename T>
size_t QRVector<T>::len() const {
    return *size;
}

template<typename T>
QRVector<T> &QRVector<T>::operator =(const QRVector<T> &newVec) {
    grow(newVec.getSize());

    VectorIterator<T> iter(arr, size);
    for (auto newIter = newVec.begin(); newIter != newVec.end(); newIter++)
        *iter = *newIter;
}

template<typename T>
QRVector<T> &QRVector<T>::operator =(const QRVector<T> &&newVec) {
    grow(newVec.getSize());
    arr = newVec.arr;
    newVec.arr.reset();

    return *this;
}

template<typename T>
QRVector<T> &QRVector<T>::operator =(std::initializer_list<T> initList) {
    grow(initList.size());

    VectorIterator<T> iter(arr, size);
    for (auto &elem : initList) {
        *iter = elem;
        iter++;
    }

    return *this;
}

template<typename T>
bool QRVector<T>::operator==(const QRVector<T> &vec) const {
    bool equalFlag = true;

    if (*size != vec.getSize())
        return false;

    auto iter = begin();
    auto vecIter = vec.begin();

    while (iter != end() && equalFlag) {
        equalFlag = *iter == *vecIter;
        vecIter++;
        iter++;
    }

    return equalFlag;
}

template<typename T>
bool QRVector<T>::operator!=(const QRVector<T> &vec) const {
    return !(*this == vec);
}

template<typename T>
bool QRVector<T>::isEqual(const QRVector<T> &vec) const {
    return *this == vec;
}

template<typename T>
bool QRVector<T>::isNotEqual(const QRVector<T> &vec) const {
    return *this != vec;
}

#endif //QRVECTOR_H
