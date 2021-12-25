//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_QRMULTIVECTORITERATOR_H
#define BIG3DFLUFFY_QRMULTIVECTORITERATOR_H

#endif //BIG3DFLUFFY_QRMULTIVECTORITERATOR_H

#include "QRVectorIterator.h"

template <typename T>
class QRDuoVectorIterator: public QRVectorIterator<T> {
public:
    QRDuoVectorIterator(sptr<QRVector<T>> a, sptr<QRVector<T>> b,
                        bool is_end = false);
    QRDuoVectorIterator(const QRDuoVectorIterator<T>& it): v1(it.v1), v2(it.v2),
            cur_vector(it.cur_vector), cur_index(it.cur_index) {this->index = it.index;}
    QRDuoVectorIterator<T>& operator=(const QRDuoVectorIterator<T>& iter);

    virtual T& operator *();
    virtual const T& operator *() const;
    virtual T* operator ->();
    virtual const T* operator ->() const;

    virtual QRVectorIterator<T>& operator ++();
    virtual QRVectorIterator<T> operator ++(int);

private:
    wptr<QRVector<T>> v1, v2;
    wptr<QRVector<T>> cur_vector;
    bool is_v1;
    size_t cur_index;
};

using namespace std;

template <typename T>
QRDuoVectorIterator<T>::QRDuoVectorIterator(sptr<QRVector<T>> a, sptr<QRVector<T>> b,
                                            bool is_end)
: v1(a), v2(b) {
    if (is_end) {
        this->index = a.get()->len() + b.get()->len();
        cur_vector = v2;
        cur_index = b.get()->len();
        is_v1 = false;
    }
    else {
        cur_vector = v1;
        this->index = 0;
        cur_index = 0;
        is_v1 = true;
    }
}

template <typename T>
T& QRDuoVectorIterator<T>::operator *() {
    if (cur_vector.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
    sptr<QRVector<T>> copyPtr(cur_vector);
    return copyPtr.get()->operator[](cur_index);
}

template <typename T>
const T& QRDuoVectorIterator<T>::operator *() const {
    if (cur_vector.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
    sptr<QRVector<T>> copyPtr(cur_vector);
    return copyPtr.get()->operator[](cur_index);
}

template<typename T>
T* QRDuoVectorIterator<T>::operator->() {
    if (cur_vector.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
    sptr<QRVector<T>> copyPtr(cur_vector);
    return &copyPtr.get()->operator[](cur_index);
}

template<typename T>
const T* QRDuoVectorIterator<T>::operator->() const{
    if (cur_vector.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
    sptr<QRVector<T>> copyPtr(cur_vector);
    return &copyPtr.get()->operator[](cur_index);
}

template<typename T>
QRVectorIterator<T>& QRDuoVectorIterator<T>::operator ++() {
    if (cur_vector.expired())
        throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);

    if (cur_index == cur_vector.lock().get()->len()) {
        if (is_v1) {
            cur_vector = v2, is_v1 = false, cur_index = 0;
            if (cur_vector.expired())
                throw ErrorDeletedObj(__FILE__, __LINE__, "vector not exists", __TIME__);
            this->index++;
        }
    }
    else {
        this->index++;
        cur_index++;
    }
    return *this;
}

template<typename T>
QRVectorIterator<T> QRDuoVectorIterator<T>::operator ++(int) {
    auto it(*this);
    ++(*this);
    return it;
}