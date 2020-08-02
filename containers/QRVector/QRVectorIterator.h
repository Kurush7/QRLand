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
    //  todo delete friend class QRVector<T>;

    QRVectorIterator() {}
    QRVectorIterator(const sptr<T[]> vect, const sptr<size_t> sz, size_t ind = 0);
    QRVectorIterator(std::initializer_list<T>);
    QRVectorIterator(const QRVectorIterator<T>& iter);
    QRVectorIterator<T>& operator=(const QRVectorIterator<T>& iter);

    ~QRVectorIterator() {}

    virtual T& operator *();
    virtual const T& operator *() const;
    virtual T* operator ->();
    virtual const T* operator ->() const;

    virtual explicit operator bool() const {
        if (!is_local) {
            if (size.expired())
                throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
            return index < *size.lock();
        }
        else {
            return index < local_size;
        }
    }   // todo not tested

    virtual QRVectorIterator<T>& operator ++();
    virtual QRVectorIterator<T> operator ++(int);
    virtual QRVectorIterator<T>& operator +(int);

    bool operator ==(const QRVectorIterator<T>& iter) const;
    bool operator !=(const QRVectorIterator<T>& iter) const;

    size_t index = 0;
protected:
    wptr<T[]> vec;
    wptr<size_t> size;

    sptr<T[]> local_arr = nullptr;
    size_t local_size=0;
    bool is_local = false;
};

template<typename T>
QRVectorIterator<T>::QRVectorIterator(const sptr<T[]> vect, const sptr<size_t> sz, size_t ind) {
    vec = wptr<T[]>(vect);
    size = wptr<size_t>(sz);
    index = ind;
}

template<typename T>
QRVectorIterator<T>::QRVectorIterator(std::initializer_list<T> lst) {
    local_arr = sptr<T[]>(new T[lst.size()]);
    local_size = lst.size();
    is_local = true;
    size_t i = 0;
    for (auto &elem: lst)
        local_arr[i++] = elem;

}

template<typename T>
QRVectorIterator<T>::QRVectorIterator(const QRVectorIterator<T> &iter)
: vec(iter.vec), size(iter.size), index(iter.index),
local_arr(iter.local_arr), local_size(iter.local_size), is_local(iter.is_local) {}

template<typename T>
QRVectorIterator<T>& QRVectorIterator<T>::operator=(const QRVectorIterator<T> &iter) {
    vec = iter.vec;
    size = iter.size;
    index = iter.index;
    local_arr = iter.local_arr;     // todo not full copy
    local_size = iter.local_size;
    is_local = iter.is_local;
    return *this;
}

template<typename T>
QRVectorIterator<T>& QRVectorIterator<T>::operator ++() {
    if (!is_local) {
        if (size.expired())
            throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
        if (index < *size.lock()) index++;
    }
    else {
        if (index < local_size) index++;
    }
    return *this;
}

template<typename T>
QRVectorIterator<T>& QRVectorIterator<T>::operator +(int x) {
    if (!is_local) {
        if (size.expired())
            throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
        if (index + x < *size.lock()) index += x;
        else index = *size.lock();
    }
    else {
        if (index+x < local_size) index += x;
        else index = local_size;
    }
    return *this;
}

template<typename T>
QRVectorIterator<T> QRVectorIterator<T>::operator ++(int) {
    auto it(*this);
    if (!is_local) {
        if (size.expired())
            throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
        if (index < *size.lock()) ++(*this);
    }
    else {
        ++(*this);
    }
    return it;
}

template<typename T>
T& QRVectorIterator<T>::operator*() {
    if (!is_local) {
        if (vec.expired())
            throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
        sptr<T[]> copyPtr(vec);
        return copyPtr[index];
    }
    else {
        return local_arr[index];
    }
}

template<typename T>
T* QRVectorIterator<T>::operator->() {
    if (!is_local) {
        if (vec.expired())
            throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
        sptr<T[]> copyPtr(vec);
        return &copyPtr[index];
    }
    else {
        return &local_arr[index];
    }
}

template<typename T>
const T& QRVectorIterator<T>::operator*() const {
    if (!is_local) {
        if (vec.expired())
            throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
        sptr<T[]> copyPtr(vec);
        return copyPtr[index];
    }
    else {
        return local_arr[index];
    }
}

template<typename T>
const T* QRVectorIterator<T>::operator->() const {
    if (!is_local) {
        if (vec.expired())
            throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
        sptr<T[]> copyPtr(vec);
        return &copyPtr[index];
    }
    else {
        return &local_arr[index];
    }
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
