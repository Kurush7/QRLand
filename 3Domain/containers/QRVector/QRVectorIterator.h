//
// Created by Георгий Куликов on 28.04.2020.
//

#ifndef LAB2_VECTORITERATOR_H
#define LAB2_VECTORITERATOR_H

#include <iterator>

#include "../exceptions/VectorError/Errors.h"

template <typename T>
class QRVectorIterator: public std::iterator<std::output_iterator_tag, T> {
private:
    std::weak_ptr<T[]> vec;
    std::weak_ptr<size_t> size;
    size_t index = 0;
public:
    QRVectorIterator(const std::shared_ptr<T[]> vect, const std::shared_ptr<size_t> sz, size_t ind = 0);
    QRVectorIterator(const QRVectorIterator<T>& iter);
    QRVectorIterator<T>& operator=(const QRVectorIterator<T>& iter);

    T& operator *();
    const T& operator *() const;
    T* operator ->();
    const T* operator ->() const;

    QRVectorIterator<T>& operator ++();
    QRVectorIterator<T> operator ++(int);

    QRVectorIterator<T>& operator --();
    const QRVectorIterator<T> operator --(int);
    QRVectorIterator<T>& operator -=(size_t ind);
    QRVectorIterator<T> operator -(size_t ind);

    bool operator ==(const QRVectorIterator<T>& iter) const;
    bool operator !=(const QRVectorIterator<T>& iter) const;
};

template<typename T>
QRVectorIterator<T>& QRVectorIterator<T>::operator ++() {
    index++;
    return *this;
}

template<typename T>
QRVectorIterator<T> QRVectorIterator<T>::operator ++(int) {
    auto it(*this);
    ++(*this);
    return it;
}


template<typename T>
QRVectorIterator<T>::QRVectorIterator(const std::shared_ptr<T[]> vect, const std::shared_ptr<size_t> sz, size_t ind) {
    vec = std::weak_ptr<T[]>(vect);
    size = std::weak_ptr<size_t>(sz);
    index = ind;
}

template<typename T>
QRVectorIterator<T>::QRVectorIterator(const QRVectorIterator<T> &iter) {
    vec = iter.vec;
    size = iter.size;
    index = iter.index;
}

template<typename T>
QRVectorIterator<T> &QRVectorIterator<T>::operator=(const QRVectorIterator<T> &iter) {
    vec = iter.vec;
    size = iter.size;
    index = iter.index;

    return *this;
}

template<typename T>
T& QRVectorIterator<T>::operator*() {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, "vector not already exist", __TIME__);
    std::shared_ptr<T[]> copyPtr(vec);
    return copyPtr[index];
}

template<typename T>
T* QRVectorIterator<T>::operator->() {
    return &(*this);
}

template<typename T>
const T& QRVectorIterator<T>::operator*() const {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");
    std::shared_ptr<T[]> copyPtr(vec);
    return copyPtr[index];
}

template<typename T>
const T* QRVectorIterator<T>::operator->() const {
    return &(*this);
}

template<typename T>
bool QRVectorIterator<T>::operator==(const QRVectorIterator<T> &iter) const {
    if (vec.expired() || iter.vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");
    return index == iter.index;
}

template<typename T>
bool QRVectorIterator<T>::operator!=(const QRVectorIterator<T> &iter) const {
    if (vec.expired() || iter.vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");
    return index != iter.index;
}


#endif //LAB2_VECTORITERATOR_H
