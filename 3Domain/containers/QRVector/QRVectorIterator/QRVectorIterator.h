//
// Created by Георгий Куликов on 28.04.2020.
//

#ifndef LAB2_VECTORITERATOR_H
#define LAB2_VECTORITERATOR_H

#include <iterator>

#include "../../QRExceptions/VectorError/Errors.h"

template <typename T>
class VectorIterator: public std::iterator<std::output_iterator_tag, T> {
private:
    std::weak_ptr<T[]> vec;
    std::weak_ptr<size_t> size;
    size_t index = 0;
public:
    VectorIterator(const std::shared_ptr<T[]> vect, const std::shared_ptr<size_t> sz, size_t ind = 0);
    VectorIterator(const VectorIterator<T>& iter);
    VectorIterator<T>& operator=(const VectorIterator<T>& iter);

    T& operator *();
    const T& operator *() const;
    T* operator ->();
    const T* operator ->() const;

    VectorIterator<T>& operator ++();
    VectorIterator<T> operator ++(int);

    VectorIterator<T>& operator --();
    const VectorIterator<T> operator --(int);
    VectorIterator<T>& operator -=(size_t ind);
    VectorIterator<T> operator -(size_t ind);

    bool operator ==(const VectorIterator<T>& iter) const;
    bool operator !=(const VectorIterator<T>& iter) const;
};

template<typename T>
VectorIterator<T>& VectorIterator<T>::operator ++() {
    index++;
    return *this;
}

template<typename T>
VectorIterator<T> VectorIterator<T>::operator ++(int) {
    auto it(*this);
    ++(*this);
    return it;
}


template<typename T>
VectorIterator<T>::VectorIterator(const std::shared_ptr<T[]> vect, const std::shared_ptr<size_t> sz, size_t ind) {
    vec = std::weak_ptr<T[]>(vect);
    size = std::weak_ptr<size_t>(sz);
    index = ind;
}

template<typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T> &iter) {
    vec = iter.vec;
    size = iter.size;
    index = iter.index;
}

template<typename T>
VectorIterator<T> &VectorIterator<T>::operator=(const VectorIterator<T> &iter) {
    vec = iter.vec;
    size = iter.size;
    index = iter.index;

    return *this;
}

template<typename T>
T& VectorIterator<T>::operator*() {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, "vector not already exist", __TIME__);
    std::shared_ptr<T[]> copyPtr(vec);
    return copyPtr[index];
}

template<typename T>
T* VectorIterator<T>::operator->() {
    return &(*this);
}

template<typename T>
const T& VectorIterator<T>::operator*() const {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");
    std::shared_ptr<T[]> copyPtr(vec);
    return copyPtr[index];
}

template<typename T>
const T* VectorIterator<T>::operator->() const {
    return &(*this);
}

template<typename T>
bool VectorIterator<T>::operator==(const VectorIterator<T> &iter) const {
    if (vec.expired() || iter.vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");
    return index == iter.index;
}

template<typename T>
bool VectorIterator<T>::operator!=(const VectorIterator<T> &iter) const {
    if (vec.expired() || iter.vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");
    return index != iter.index;
}


#endif //LAB2_VECTORITERATOR_H
