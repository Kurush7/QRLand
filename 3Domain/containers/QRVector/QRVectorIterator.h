//
// Created by Георгий Куликов on 28.04.2020.
//

#ifndef LAB2_VECTORITERATOR_H
#define LAB2_VECTORITERATOR_H

#include <iterator>

#include "../exceptions/VectorError/Errors.h"
#include "QRVector.h"

template <typename T>
class QRVectorIterator: public std::iterator<std::output_iterator_tag, T> {
public:
    friend class QRVector<T>;

    QRVectorIterator() {}
    QRVectorIterator(const std::shared_ptr<T[]> vect, const std::shared_ptr<size_t> sz, size_t ind = 0);
    QRVectorIterator(const QRVectorIterator<T>& iter);
    QRVectorIterator<T>& operator=(const QRVectorIterator<T>& iter);

    virtual T& operator *();
    virtual const T& operator *() const;
    virtual T* operator ->();
    virtual const T* operator ->() const;

    virtual QRVectorIterator<T>& operator ++();
    virtual QRVectorIterator<T> operator ++(int);

    bool operator ==(const QRVectorIterator<T>& iter) const;
    bool operator !=(const QRVectorIterator<T>& iter) const;

private:
    std::weak_ptr<T[]> vec;
    std::weak_ptr<size_t> size;
protected:
    size_t index = 0;
};

template<typename T>
QRVectorIterator<T>::QRVectorIterator(const std::shared_ptr<T[]> vect, const std::shared_ptr<size_t> sz, size_t ind) {
    vec = std::weak_ptr<T[]>(vect);
    size = std::weak_ptr<size_t>(sz);
    index = ind;
}

template<typename T>
QRVectorIterator<T>::QRVectorIterator(const QRVectorIterator<T> &iter)
: vec(iter.vec), size(iter.size), index(iter.index) {}

template<typename T>
QRVectorIterator<T>& QRVectorIterator<T>::operator=(const QRVectorIterator<T> &iter) {
    vec = iter.vec;
    size = iter.size;
    index = iter.index;
    return *this;
}


template<typename T>
QRVectorIterator<T>& QRVectorIterator<T>::operator ++() {
    if (size.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
    if(index < *size.lock()) index++;
    return *this;
}

template<typename T>
QRVectorIterator<T> QRVectorIterator<T>::operator ++(int) {
    auto it(*this);
    if (size.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
    if(index < *size.lock()) ++(*this);
    return it;
}

template<typename T>
T& QRVectorIterator<T>::operator*() {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
    std::shared_ptr<T[]> copyPtr(vec);
    return copyPtr[index];
}

template<typename T>
T* QRVectorIterator<T>::operator->() {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
    std::shared_ptr<T[]> copyPtr(vec);
    return &copyPtr[index];
}

template<typename T>
const T& QRVectorIterator<T>::operator*() const {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not exists");
    std::shared_ptr<T[]> copyPtr(vec);
    return copyPtr[index];
}

template<typename T>
const T* QRVectorIterator<T>::operator->() const {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not exists");
    std::shared_ptr<T[]> copyPtr(vec);
    return &copyPtr[index];
}

template<typename T>
bool QRVectorIterator<T>::operator==(const QRVectorIterator<T> &iter) const {
    //if (vec.expired() || iter.vec.expired())
    //    throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not exists");
    return index == iter.index;
}

template<typename T>
bool QRVectorIterator<T>::operator!=(const QRVectorIterator<T> &iter) const {
    //if (vec.expired() || iter.vec.expired())  // todo needed for single-component objects to work (empty iterator)
    //    throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not exists"); // todo also for children
    return index != iter.index;
}


#endif //LAB2_VECTORITERATOR_H
