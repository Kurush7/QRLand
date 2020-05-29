//
// Created by Георгий Куликов on 28.04.2020.
//

#ifndef LAB2_VECTORITERATOR_H
#define LAB2_VECTORITERATOR_H

#include <iterator>
#include "Iterator.h"
#include "../VectorError/Errors.h"

template <typename Type>
class VectorIterator: public Iterator, public std::iterator<std::input_iterator_tag, Type> {
private:
    std::weak_ptr<Type[]> vec;
public:
    VectorIterator(const std::shared_ptr<Type> vect, const std::shared_ptr<size_t> sz, size_t ind = 0);
    VectorIterator(const VectorIterator<Type>& iter);
    VectorIterator<Type>& operator=(const VectorIterator<Type>& iter);

    Type* cur();
    const Type* cur() const;
    VectorIterator<Type>& next();
    VectorIterator<Type>& prev();

    Type& operator *();
    const Type& operator *() const;
    Type* operator ->();
    const Type* operator ->() const;

    VectorIterator<Type>& operator ++();
    VectorIterator<Type> operator ++(int);
    VectorIterator<Type>& operator +=(size_t ind);
    VectorIterator<Type> operator +(size_t ind);

    VectorIterator<Type>& operator --();
    const VectorIterator<Type> operator --(int);
    VectorIterator<Type>& operator -=(size_t ind);
    VectorIterator<Type> operator -(size_t ind);

    bool operator ==(const VectorIterator<Type>& iter) const;
    bool operator !=(const VectorIterator<Type>& iter) const;
};






template<typename Type>
VectorIterator<Type>::VectorIterator(const std::shared_ptr<Type> vect, const std::shared_ptr<size_t> sz, size_t ind) {
    vec = std::weak_ptr<Type>(vect);
    size = std::weak_ptr<size_t>(sz);
    index = ind;
}

template<typename Type>
VectorIterator<Type>::VectorIterator(const VectorIterator<Type> &iter) {
    vec = iter.vec;
    size = iter.size;
    index = iter.index;
}

template<typename Type>
VectorIterator<Type> &VectorIterator<Type>::operator=(const VectorIterator<Type> &iter) {
    vec = iter.vec;
    size = iter.size;
    index = iter.index;

    return *this;
}

template<typename Type>
Type* VectorIterator<Type>::cur() {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, "vector not already exist", __TIME__);
    std::shared_ptr<Type> copyPtr(vec);
    return copyPtr.get() + index;
}

template<typename Type>
const Type* VectorIterator<Type>::cur() const {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");
    std::shared_ptr<Type[]> copyPtr(vec);
    return copyPtr.get() + index;
}

template<typename Type>
VectorIterator<Type>& VectorIterator<Type>::next() {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");
    if (index >= *size.lock()) {
        return *this;
    }

    index++;
    return *this;
}

template<typename Type>
VectorIterator<Type>& VectorIterator<Type>::prev() {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");
    if (this->index - 1 < 0) {
        return *this;
    }

    this->index--;
    return *this;
}

template<typename Type>
Type& VectorIterator<Type>::operator*() {
    return *(this->cur());
}

template<typename Type>
Type* VectorIterator<Type>::operator->() {
    return this->cur();
}

template<typename Type>
const Type& VectorIterator<Type>::operator*() const {
    return *(this->cur());
}

template<typename Type>
const Type* VectorIterator<Type>::operator->() const {
    return this->cur();
}

template<typename Type>
VectorIterator<Type>& VectorIterator<Type>::operator++() {
    return this->next();
}

template<typename Type>
VectorIterator<Type> VectorIterator<Type>::operator++(int) {
    VectorIterator<Type> it(*this);
    ++(*this);
    return it;
}

template<typename Type>
VectorIterator<Type> &VectorIterator<Type>::operator--() {
    return this->prev();
}

template<typename Type>
const VectorIterator<Type> VectorIterator<Type>::operator--(int) {
    VectorIterator<Type> it(*this);
    --(*this);
    return it;
}

template<typename Type>
VectorIterator<Type> &VectorIterator<Type>::operator+=(size_t ind) {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");

    if (this->index + ind >= *size) {
        ErrorIndex err(__FILE__, __LINE__, __TIME__, "can't get elem, index out of range");
        throw(err);
    }

    this->index += ind;
    return *this;
}

template<typename Type>
VectorIterator<Type> &VectorIterator<Type>::operator-=(size_t ind) {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");

    if (this->index - ind < 0) {
        ErrorIndex err(__FILE__, __LINE__, __TIME__, "can't get elem, index out of range");
        throw(err);
    }

    this->index += ind;
    return *this;
}

template<typename Type>
VectorIterator<Type> VectorIterator<Type>::operator+(size_t ind) {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");

    VectorIterator<Type> it(*this);
    (*this) += ind;
    return it;
}

template<typename Type>
VectorIterator<Type> VectorIterator<Type>::operator-(size_t ind) {
    if (vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");

    VectorIterator<Type> it(*this);
    (*this) -= ind;
    return it;
}

template<typename Type>
bool VectorIterator<Type>::operator==(const VectorIterator<Type> &iter) const {
    if (vec.expired() || iter.vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");
    return index == iter.index;
}

template<typename Type>
bool VectorIterator<Type>::operator!=(const VectorIterator<Type> &iter) const {
    if (vec.expired() || iter.vec.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, __TIME__, "vector not already exist");
    return index != iter.index;
}


#endif //LAB2_VECTORITERATOR_H
