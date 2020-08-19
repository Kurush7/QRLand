//
// Created by Георгий Куликов on 27.04.2020.
//

#ifndef QRVECTOR_H
#define QRVECTOR_H


#include <initializer_list>
#include <cmath>
#include <iostream>
#include "VectorBase.h"

const float DEFAULT_SIZE = 8;

template <typename T>
class QRVectorIterator;

template <typename T>
class QRVector: public VectorBase {
public:
    explicit QRVector(size_t = 0);
    QRVector(size_t, T*);
    QRVector(std::initializer_list<T>);
    QRVector(const QRVector<T> &);
    QRVector(QRVector<T>&&);

    void clear() {*size = 0;}
    void reserve(size_t n) {    // todo size not changing....
        size_t new_size = max_size, mult=0;
        while (new_size < n) new_size *= 2, mult++;
        if (mult) grow(mult);
    }

    sptr<QRVector<T>> getPointer() const {return p;}
    T* getPureArray() const {return arr.get();}

    QRVector<T>& operator =(const QRVector<T>&);
    QRVector<T>& operator =(QRVector<T>&&);
    QRVector<T>& operator =(std::initializer_list<T>);
    QRVector<T>& operator =(const QRVectorIterator<T> &it);

    QRVectorIterator<T> begin() const;
    QRVectorIterator<T> end() const;

    T& getElem(size_t index) {return arr[index];}
    const T& getElem(size_t index) const {return arr[index];}

    T& operator [](size_t index);
    const T& operator [](size_t index) const;
    void push_back(const T& newValue);
    void push_front(const T& newValue);
    T pop_back() {
        if(*size == 0)
            throw ErrorIndex(__FILE__, __LINE__, __TIME__, "pop from empty queue");
        (*size)--;
        return arr[*size];
    }
    T pop(size_t ind) {
        if(*size <= ind)
            throw ErrorIndex(__FILE__, __LINE__, __TIME__, "pop from bad index");
        T x = arr[ind];
        for (size_t i = ind; i < *size-1; ++i)
            arr[i] = arr[i+1];
        (*size)--;
        return x;
    }
    T pop(QRVectorIterator<T> it) {
        size_t ind = it.index;
        if(*size <= ind)
            throw ErrorIndex(__FILE__, __LINE__, __TIME__, "pop from bad index");
        T x = arr[ind];
        for (size_t i = ind; i < *size-1; ++i)
            arr[i] = arr[i+1];
        (*size)--;
        return x;
    }
    size_t len() const;

    void reverse();

    bool operator ==(const QRVector<T>& vec) const;
    bool operator !=(const QRVector<T>& vec) const;
    bool isEqual(const QRVector<T>& vec) const;
    bool isNotEqual(const QRVector<T>& vec) const;
private:
    sptr<T[]> arr;
    sptr<QRVector<T>> p;
    void grow(int times=1);
};

using namespace std;

template<typename T>
std::ostream& operator <<(std::ostream &os, const QRVector<T> &vec);

template<typename T>
void QRVector<T>::grow(int times) {
    try {
        max_size *= pow(2,times);
        auto new_arr = sptr<T[]>(new T[max_size]);
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
    p = sptr<QRVector>(this, [](void *ptr){});
    try {
        max_size = DEFAULT_SIZE;
        while (max_size < sz)
            max_size *= 2;
        size = sptr<size_t>(new size_t(sz));
        this->arr = sptr<T[]>(new T[max_size]);
    }
    catch (bad_alloc &exc) {
        throw ErrorAlloc(__FILE__, __LINE__, __TIME__, "can't allocate memory");
    }
}

template<typename T>
QRVector<T>::QRVector(size_t sz, T *vec): VectorBase(sz) {
    p = sptr<QRVector>(this, [](void *ptr){});
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
    p = sptr<QRVector>(this, [](void *ptr){});
    size_t i = 0;
    for (auto &elem : initList)
        arr[i++] = elem;
}

template<typename T>
QRVector<T>::QRVector(const QRVector<T> &vec): QRVector() {
    p = sptr<QRVector>(this, [](void *ptr){});
    while (max_size < vec.len())
        grow();

    size = vec.size;
    for (size_t i = 0; i < vec.len(); ++i)
        arr[i] = vec[i];
}

template<typename T>
QRVector<T>::QRVector(QRVector<T> &&vec) {
    p = sptr<QRVector>(this, [](void *ptr){});
    arr = vec.arr;
    size = vec.size;
    max_size = vec.max_size;
    vec.arr.reset();
    vec.size.reset();
}


template<typename T>
QRVectorIterator<T> QRVector<T>::begin() const {
    return QRVectorIterator<T>(arr, size);
}

template<typename T>
QRVectorIterator<T> QRVector<T>::end() const {
    return QRVectorIterator<T>(arr, size, *size);
}


template<typename T>
void QRVector<T>::push_back(const T &val) {
    if (*size >= max_size - 1) grow();
    arr[*size] = val;
    (*size)++;
}

template<typename T>
void QRVector<T>::push_front(const T &val) {
    if (*size >= max_size - 1) grow();
    for (size_t i = (*size+1); i > 0; --i)
        arr[i] = arr[i-1];
    arr[0] = val;
    (*size)++;
}


template<typename T>
T& QRVector<T>::operator[](size_t index) {
    return arr[index];
}

template<typename T>
const T& QRVector<T>::operator[](size_t index) const {
    return arr[index];
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
void QRVector<T>::reverse() {   // todo not tested
    for (size_t i = 0; i < *size / 2; ++i)
        swap(arr[i], arr[*size - i - 1]);
}

template<typename T>
QRVector<T>& QRVector<T>::operator =(const QRVector<T> &newVec) {
    while (max_size < newVec.len())
        grow();

    *size = newVec.len();
    for (size_t i = 0; i < *size; ++i)
        arr[i] = newVec[i];
}

template<typename T>
QRVector<T>& QRVector<T>::operator =(QRVector<T> &&newVec) {
    // todo
    size = newVec.size;
    max_size = newVec.max_size;
    arr = newVec.arr;
    newVec.arr.reset();

    return *this;
}

template<typename T>
QRVector<T>& QRVector<T>::operator =(std::initializer_list<T> initList) {
    grow(initList.size());

    QRVectorIterator<T> iter(arr, size);
    for (auto &elem : initList) {
        *iter = elem;
        iter++;
    }

    return *this;
}

template<typename T>
QRVector<T>& QRVector<T>::operator =(const QRVectorIterator<T> &it) {
    *size = 0;
    for (auto i = it; i; i++)
        push_back(*i);
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
