//
// Created by Георгий Куликов on 27.04.2020.
//

#ifndef LAB2_VECTOR_H
#define LAB2_VECTOR_H


#include <initializer_list>
#include <cmath>
#include <iostream>
#include "VectorBase.h"
#include "VectorIterator/VectorIterator.h"


template <typename Type>
class Vector final: public VectorBase {
private:
    std::shared_ptr<Type[]> vecPtr;
    void newMemory(size_t size);
public:
    explicit Vector(size_t sz = 0);
    Vector(size_t sz, Type* vec);
    Vector(size_t sz, Type vec, ...);
    Vector(std::initializer_list<Type> initList);

    explicit Vector(const Vector<Type> &vec);
    Vector(Vector<Type>&& vec);

    VectorIterator<Type> begin() const;
    VectorIterator<Type> end() const;

    const Type getElem(size_t index) const;
    Type getElem(size_t index);
    void setElem(size_t index, const Type& newValue);
    Type& operator [](int index);
    const Type& operator [](size_t index) const;

    void addElem(const Type& newValue);
    Type sumElem() const;

    Type length() const;
    double angleBetween(const Vector<Type>& vec);
    Vector<Type> getUnit() const;

    bool isUnit() const;
    bool isZero() const;
    bool isCollinear(const Vector<Type>& vec) const;
    bool isOrthogonal(const Vector<Type>& vec) const;

    Vector<Type>& operator =(const Vector<Type>& newVec);
    Vector<Type>& operator =(const Vector<Type>&& newVec);
    Vector<Type>& operator =(std::initializer_list<Type> initList);

    Vector<Type> operator +(const Vector<Type>& vec) const;
    Vector<Type> operator -(const Vector<Type>& vec) const;
    Vector<Type> operator -();
    Vector<Type> operator /(const Vector<Type>& vec) const;
    Vector<Type> operator *(const Vector<Type>& vec) const;

    Vector<Type> sum(const Vector<Type>& vec) const;
    Vector<Type> dif(const Vector<Type>& vec) const;
    Vector<Type> neg();
    Vector<Type> div(const Vector<Type>& vec) const;
    Vector<Type> mult(const Vector<Type>& vec) const;

    Vector<Type>& operator +=(const Vector<Type>& vec);
    Vector<Type>& operator -=(const Vector<Type>& vec);
    Vector<Type>& operator /=(const Vector<Type>& vec);
    Vector<Type>& operator *=(const Vector<Type>& vec);

    bool operator ==(const Vector<Type>& vec) const;
    bool operator !=(const Vector<Type>& vec) const;
    bool isEqual(const Vector<Type>& vec) const;
    bool isNotEqual(const Vector<Type>& vec) const;
};

template<typename Type>
std::ostream& operator <<(std::ostream &os, const Vector<Type> &vec);







const double EPS = 1e-8;

template<typename Type>
void Vector<Type>::newMemory(size_t sz) {
    this->size = std::shared_ptr<size_t>(new size_t(sz));
    this->vecPtr = std::shared_ptr<Type>(new Type[sz]);

    if (!this->vecPtr || !this->size)
        throw ErrorAlloc(__FILE__, __LINE__, __TIME__, "can't allocate memory");
}

template<typename Type>
Vector<Type>::Vector(size_t sz):VectorBase(sz) {
    this->vecPtr = std::shared_ptr<Type>(new Type[sz]);
    if (!this->vecPtr)
        throw ErrorAlloc(__FILE__, __LINE__, __TIME__, "can't allocate memory");
}

template<typename Type>
Vector<Type>::Vector(size_t sz, Type *vec):VectorBase(sz) {
    if (!vec)
        throw ErrorNullptr(__FILE__, __LINE__, __TIME__, "can't initialize vector with nullptr");
    vecPtr = std::shared_ptr<Type>(new Type[sz]);
    if (!this->vecPtr)
        throw ErrorAlloc(__FILE__, __LINE__, __TIME__, "can't allocate memory");

    size_t i = 0;
    for (auto& elem : *this) {
        elem = vec[i];
        i++;
    }
}

template<typename Type>
Vector<Type>::Vector(size_t sz, Type vec, ...):VectorBase(sz) {
    newMemory(sz);
    va_list ap;
    va_start(ap, vec);

    for (auto& elem : *this) {
        elem = vec;
        vec = va_arg(ap, Type);
    }

    va_end(ap);
}

template<typename Type>
Vector<Type>::Vector(std::initializer_list<Type> initList) {
    newMemory(initList.size());
    VectorIterator<Type> iter(vecPtr, size);

    for (auto &elem : initList) {
        *iter = elem;
        iter++;
    }
}

template<typename Type>
Vector<Type>::Vector(const Vector<Type> &vec) {
    newMemory(vec.getSize());

    VectorIterator<Type> iter = vec.begin();
    for (auto& elem : *this) {
        elem = *iter;
        *iter++;
    }
}

template<typename Type>
Vector<Type>::Vector(Vector<Type> &&vec) {
    newMemory(vec.getSize());
    vecPtr = vec.vecPtr;
    vec.vecPtr.reset();
}



template<typename Type>
VectorIterator<Type> Vector<Type>::begin() const {
    return VectorIterator<Type>(vecPtr, size);
}

template<typename Type>
VectorIterator<Type> Vector<Type>::end() const {
    return VectorIterator<Type>(vecPtr, size, *size);
}



template<typename Type>
const Type Vector<Type>::getElem(size_t index) const {
    if (index >= *size)
        throw ErrorIndex(__FILE__, __LINE__, __TIME__, "can't get elem, index out of range");
    VectorIterator<Type> iter(vecPtr, size, index);
    return *iter;
}

template<typename Type>
Type Vector<Type>::getElem(size_t index) {
    if (index >= *size)
        throw ErrorIndex(__FILE__, __LINE__, __TIME__, "can't get elem, index out of range");
    VectorIterator<Type> iter(vecPtr, size, index);
    return *iter;
}

template<typename Type>
void Vector<Type>::setElem(size_t index, const Type &newValue) {
    if (index >= *size)
        throw ErrorIndex(__FILE__, __LINE__, __TIME__, "can't set elem, index out of range");
    VectorIterator<Type> iter(vecPtr, size, index);
    *iter = newValue;
}

template<typename Type>
void Vector<Type>::addElem(const Type &newValue) {
    Vector<Type> buf(*this);
    newMemory(*size + 1);

    VectorIterator<Type> iter = begin();
    for (auto& elem : buf) {
        *iter = elem;
        iter++;
    }

    *iter = newValue;
}

template<typename Type>
Type &Vector<Type>::operator[](int index) {
    return this->getElem(index);
}

template<typename Type>
const Type &Vector<Type>::operator[](size_t index) const {
    return this->getElem(index);
}



template<typename Type>
std::ostream& operator <<(std::ostream &os, const Vector<Type> &vec) {
    if (vec.isEmpty()) {
        os << "Vector is empty";
        return os;
    }
    VectorIterator<Type> iter = vec.begin();

    os << '(';
    os << *iter;
    iter++;
    for (; iter != vec.end(); iter++) {
        os << ", " << *iter;
    }
    os << ')';

    return os;
}



template<typename Type>
Type Vector<Type>::length() const {
    Type sum = 0;
    for (auto& elem : *this)
        sum += elem * elem;

    return sqrt(sum);
}



template<typename Type>
bool Vector<Type>::isUnit() const {
    return abs(this->length() - 1) < EPS;
}

template<typename Type>
Vector<Type> Vector<Type>::getUnit() const {
    Vector<Type> newVec(*this);
    Type len = newVec.length();

    for (auto& elem : newVec)
        elem /= len;

    return newVec;
}

template<typename Type>
bool Vector<Type>::isZero() const {
    return abs(this->length()) < EPS;
}

template<typename Type>
bool Vector<Type>::isCollinear(const Vector<Type>& vec) const {
    return abs(this->angleBetween(vec)) < EPS;
}

template<typename Type>
bool Vector<Type>::isOrthogonal(const Vector<Type>& vec) const {
    return abs(this->angleBetween(vec) - 90) < EPS;
}



template<typename Type>
double Vector<Type>::angleBetween(const Vector<Type> &vec) {
    Type len = this->length();
    Type vecLen = vec.length();
    if (len == 0 || vecLen == 0)
        throw ErrorZeroDivision(__FILE__, __LINE__, __TIME__, "zero division, one of vector has zero length while calculating angle");

    double angle = (*this * vec) / (len * vecLen);
    return acos(angle) * 180 / M_PI;
}

template<typename Type>
Type Vector<Type>::sumElem() const {
    Type sum = 0;
    for (auto &elem : *this)
        sum += elem;
    return sum;
}



template<typename Type>
Vector<Type> &Vector<Type>::operator =(const Vector<Type> &newVec) {
    newMemory(newVec.getSize());

    VectorIterator<Type> iter(vecPtr, size);
    for (auto newIter = newVec.begin(); newIter != newVec.end(); newIter++)
        *iter = *newIter;
}

template<typename Type>
Vector<Type> &Vector<Type>::operator =(const Vector<Type> &&newVec) {
    newMemory(newVec.getSize());
    vecPtr = newVec.vecPtr;
    newVec.vecPtr.reset();

    return *this;
}

template<typename Type>
Vector<Type> &Vector<Type>::operator =(std::initializer_list<Type> initList) {
    newMemory(initList.size());

    VectorIterator<Type> iter(vecPtr, size);
    for (auto &elem : initList) {
        *iter = elem;
        iter++;
    }

    return *this;
}



template<typename Type>
Vector<Type> Vector<Type>::operator +(const Vector<Type> &vec) const {
    if (*size != vec.getSize())
        throw ErrorSize(__FILE__, __LINE__, __TIME__, "can't get summary of vectors, cause of different sizes");
    Vector<Type> newVec(*this);

    auto iter = this->begin();
    auto vecIter = vec.begin();
    auto newIter = newVec.begin();

    while (iter != this->end()) {
        *newIter = *iter + *vecIter;
    }

    return newVec;
}

template<typename Type>
Vector<Type> Vector<Type>::operator -(const Vector<Type> &vec) const {
    if (*size != vec.getSize())
        throw ErrorSize(__FILE__, __LINE__, __TIME__, "can't get difference of vectors, cause of different sizes");
    Vector<Type> newVec(*this);

    auto iter = this->begin();
    auto vecIter = vec.begin();
    auto newIter = newVec.begin();

    while (iter != this->end()) {
        *newIter = *iter - *vecIter;
    }

    return newVec;
}

template<typename Type>
Vector<Type> Vector<Type>::operator -() {
    Vector<Type> newVec(*this);
    for (auto iter = newVec.begin(); iter != newVec.end(); iter++)
        *iter = -(*iter);
    return newVec;
}

template<typename Type>
Vector<Type> Vector<Type>::operator /(const Vector<Type> &vec) const {
    if (*size != vec.getSize())
        throw ErrorSize(__FILE__, __LINE__, __TIME__, "can't get division of vectors, cause of different sizes");
    Vector<Type> newVec(*this);

    auto iter = this->begin();
    auto vecIter = vec.begin();
    auto newIter = newVec.begin();

    while (iter != this->end()) {
        if (*vecIter == 0)
            throw ErrorZeroDivision(__FILE__, __LINE__, __TIME__, "can't divide vectors, zero division error");
        *newIter = *iter / *vecIter;
    }

    return newVec;
}

template<typename Type>
Vector<Type> Vector<Type>::operator *(const Vector<Type> &vec) const {
    if (*size != vec.getSize())
        throw ErrorSize(__FILE__, __LINE__, __TIME__, "can't get multiplication of vectors, cause of different sizes");
    Vector<Type> newVec(*this);

    auto iter = this->begin();
    auto vecIter = vec.begin();
    auto newIter = newVec.begin();

    while (iter != this->end()) {
        *newIter = *iter * *vecIter;
    }

    return newVec;
}



template<typename Type>
Vector<Type> Vector<Type>::sum(const Vector<Type> &vec) const {
    return *this + vec;
}

template<typename Type>
Vector<Type> Vector<Type>::dif(const Vector<Type> &vec) const {
    return *this - vec;
}

template<typename Type>
Vector<Type> Vector<Type>::neg() {
    return -(*this);
}

template<typename Type>
Vector<Type> Vector<Type>::div(const Vector<Type> &vec) const {
    return *this / vec;
}

template<typename Type>
Vector<Type> Vector<Type>::mult(const Vector<Type> &vec) const {
    return *this * vec;
}



template<typename Type>
Vector<Type> &Vector<Type>::operator +=(const Vector<Type> &vec) {
    if (*size != vec.getSize())
        throw ErrorSize(__FILE__, __LINE__, __TIME__, "can't get summary of vectors, cause of different sizes");

    auto vecIter = vec.begin();

    for (auto& elem : *this) {
        elem += *vecIter;
        vecIter++;
    }

    return *this;
}

template<typename Type>
Vector<Type> &Vector<Type>::operator -=(const Vector<Type> &vec) {
    if (*size != vec.getSize())
        throw ErrorSize(__FILE__, __LINE__, __TIME__, "can't get difference of vectors, cause of different sizes");

    auto vecIter = vec.begin();

    for (auto& elem : *this) {
        elem -= *vecIter;
        vecIter++;
    }

    return *this;
}

template<typename Type>
Vector<Type> &Vector<Type>::operator /=(const Vector<Type> &vec) {
    if (*size != vec.getSize())
        throw ErrorSize(__FILE__, __LINE__, __TIME__, "can't get division of vectors, cause of different sizes");

    auto vecIter = vec.begin();

    for (auto& elem : *this) {
        if (*vecIter == 0)
            throw ErrorZeroDivision(__FILE__, __LINE__, __TIME__, "can't divide vectors, zero division error");
        elem /= *vecIter;
        vecIter++;
    }

    return *this;
}

template<typename Type>
Vector<Type> &Vector<Type>::operator *=(const Vector<Type> &vec) {
    if (*size != vec.getSize())
        throw ErrorSize(__FILE__, __LINE__, __TIME__, "can't get multiplication of vectors, cause of different sizes");

    auto vecIter = vec.begin();

    for (auto& elem : *this) {
        elem *= *vecIter;
        vecIter++;
    }

    return *this;
}



template<typename Type>
bool Vector<Type>::operator==(const Vector<Type> &vec) const {
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

template<typename Type>
bool Vector<Type>::operator!=(const Vector<Type> &vec) const {
    return !(*this == vec);
}

template<typename Type>
bool Vector<Type>::isEqual(const Vector<Type> &vec) const {
    return *this == vec;
}

template<typename Type>
bool Vector<Type>::isNotEqual(const Vector<Type> &vec) const {
    return *this != vec;
}

#endif //LAB2_VECTOR_H
