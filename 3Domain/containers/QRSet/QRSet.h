//
// Created by kurush on 01.05.2020.
//

#ifndef OOP_SET_H
#define OOP_SET_H

#include <ctime>

#include "../QRHashTable/QRHashTable.h"
#include "QRHashTable/QRHashTableIterator.h"
#include "../QRExceptions/SetError.h"
#include "QRSetBase.h"

template <typename T>
class QRSet: public QRSetBase {
public:
    QRSet() = default;
    QRSet(const QRSet &);
    QRSet(QRSet &&) noexcept;
    QRSet(const std::initializer_list<T> &);
    QRSet(const T*, size_t);

    QRSet& operator=(const QRSet &);
    QRSet& operator=(QRSet &&) noexcept;
    QRSet& operator=(const std::initializer_list<T> &);

    virtual ~QRSet() = default;

    QRHashTableIterator<T> begin() const;
    QRHashTableIterator<T> end() const;

    T* make_array() const;

    void clear() noexcept;
    QRHashTableIterator<T> add(const T &);
    QRHashTableIterator<T> erase(const T &);
    bool has(const T &) const noexcept;
    size_t size() const noexcept;
    bool isEmpty() const noexcept;

    QRSet join(const QRSet&) const;
    QRSet join(const T&) const;
    QRSet& join_update(const QRSet&);
    QRSet& join_update(const T&);
    QRSet& operator |=(const QRSet&);
    QRSet& operator |=(const T&);

    QRSet intersection(const QRSet&) const;
    QRSet intersection(const T&) const;
    QRSet& intersection_update(const QRSet&);
    QRSet& intersection_update(const T&);
    QRSet& operator &=(const QRSet&);
    QRSet& operator &=(const T&);

    QRSet difference(const QRSet&) const;
    QRSet difference(const T&) const;
    QRSet& difference_update(const QRSet&);
    QRSet& difference_update(const T&);
    QRSet& operator -=(const QRSet&);
    QRSet& operator -=(const T&);

    QRSet sym_difference(const QRSet&) const;
    QRSet sym_difference(const T&) const;
    QRSet& sym_difference_update(const QRSet&);
    QRSet& sym_difference_update(const T&);
    QRSet& operator ^=(const QRSet&);
    QRSet& operator ^=(const T&);

    bool isSubSet(const QRSet&) const;
    bool isSuperSet(const QRSet&) const;

    bool isEqual(const QRSet&) const;
    bool isNotEqual(const QRSet&) const;

private:
    QRHashTable<T> hashTable;
};

using namespace std;

template <typename T>
QRSet<T>::QRSet(const QRSet<T> &s): hashTable(s.hashTable) {}

template <typename T>
QRSet<T>::QRSet(QRSet<T> &&s) noexcept: hashTable(move(s.hashTable)) {}

template <typename T>
QRSet<T>::QRSet(const std::initializer_list<T> &list): QRSet() {
    for (auto el: list)
        hashTable.add(el);
}

template <typename T>
QRSet<T>::QRSet(const T* arr, size_t n): QRSet() {
    if (!arr && n) {
        time_t t = time(NULL);
        throw QRSetExceptionBadPointer(__FILE__, __LINE__, asctime(localtime(&t)),
                                       "bad pointer for array-constructor!");
    }

    for (size_t i = 0; i < n; ++i)
        hashTable.add(arr[i]);
}

template <typename T>
QRSet<T>& QRSet<T>::operator=(const QRSet &s) {
    hashTable = s.hashTable;
}

template <typename T>
QRSet<T>& QRSet<T>::operator=(QRSet &&s) noexcept {
    hashTable = move(s.hashTable);
}

template <typename T>
QRSet<T>& QRSet<T>::operator=(const initializer_list<T> &list) {
    hashTable.clear();
    for (auto el: list)
        hashTable.add(el);
}


template <typename T>
QRHashTableIterator<T> QRSet<T>::begin() const { return hashTable.begin(); }

template <typename T>
QRHashTableIterator<T> QRSet<T>::end() const { return hashTable.end(); }


template <typename T>
T* QRSet<T>::make_array() const{
    try {
        size_t n = size();
        T *arr = new T[n];

        size_t i = 0;
        for (auto x: hashTable)
            arr[i++] = x;

        return arr;
    }
    catch (bad_alloc &exc) {
        time_t t = time(NULL);
        throw QRBadAllocException(__FILE__, __LINE__, asctime(localtime(&t)), "Set failed to create an array!", size() * sizeof(T));
    }
}


template <typename T>
void QRSet<T>::clear() noexcept { hashTable.clear(); }

template <typename T>
QRHashTableIterator<T> QRSet<T>::add(const T &x) { return hashTable.add(x); }

template <typename T>
QRHashTableIterator<T> QRSet<T>::erase(const T &x) { return hashTable.erase(x); }

template <typename T>
bool QRSet<T>::has(const T &x) const noexcept { return hashTable.has(x); }

template <typename T>
size_t QRSet<T>::size() const noexcept { return hashTable.size(); }

template <typename T>
bool QRSet<T>::isEmpty() const noexcept { return hashTable.size() == 0; }


template <typename T>
bool QRSet<T>::isEqual(const QRSet<T> &a) const {
    if (size() != a.size())
        return false;

    bool flag = true;
    for (auto it = begin(); it != end() && flag; ++it)
        flag &= a.has(*it);

    return flag;
}

template <typename T>
bool operator ==(const QRSet<T> &a, const QRSet<T> &b) {
    return a.isEqual(b);
}

template <typename T>
bool QRSet<T>::isNotEqual(const QRSet<T> &a) const {
    return !(isEqual(a));
}

template <typename T>
bool operator !=(const QRSet<T> &a, const QRSet<T> &b) {
    return a.isNotEqual(b);
}


template <typename T>
bool QRSet<T>::isSubSet(const QRSet<T> &s) const {
    bool flag = true;
    for (auto it = this->begin(); it != this->end() && flag; ++it)
        if (!s.has(*it))
            flag = false;
    return flag;
}

template <typename  T>
bool operator <=(const QRSet<T> &a, const QRSet<T> &b) {
    return a.isSubSet(b);
}

template <typename T>
bool QRSet<T>::isSuperSet(const QRSet<T> &s) const {
    bool flag = true;
    for (auto x: s)
        if (!has(x))
            flag = false;
    return flag;
}

template <typename  T>
bool operator >=(const QRSet<T> &a, const QRSet<T> &b) {
    return a.isSuperSet(b);
}


template <typename T>
QRSet<T> QRSet<T>::join(const QRSet<T> &s) const {
    QRSet<T> res(*this);
    for (auto x: s)
        res.add(x);
    return res;
}

template <typename T>
QRSet<T> QRSet<T>::join(const T &x) const {
    QRSet<T> res(*this);
    res.add(x);
    return res;
}

template <typename T>
QRSet<T>& QRSet<T>::join_update(const QRSet<T> &s) {
    for (auto x: s)
        if (!has(x))
            add(x);
    return *this;
}

template <typename T>
QRSet<T>& QRSet<T>::join_update(const T &x) {
    add(x);
    return *this;
}

template <typename T>
QRSet<T>& QRSet<T>::operator |=(const QRSet<T> &s) {
    return join_update(s);
}

template <typename T>
QRSet<T>& QRSet<T>::operator |=(const T &x) {
    return join_update(x);
}

template <typename T>
QRSet<T> operator |(const QRSet<T> &a, const QRSet<T> &b) {
    return a.join(b);
}

template <typename T>
QRSet<T> operator |(const T &x, const QRSet<T> &a) {
    return a.join(x);
}

template <typename T>
QRSet<T> operator |(const QRSet<T> &a, const T &x) {
    return a.join(x);
}


template <typename T>
QRSet<T> QRSet<T>::intersection(const QRSet &s) const {
    QRSet<T> res(*this);
    for (auto x: *this)
        if (!s.has(x))
            res.erase(x);
    return res;
}

template <typename T>
QRSet<T> QRSet<T>::intersection(const T &x) const {
    QRSet<T> res;
    if (this->has(x))
        res.add(x);
    return res;
}

template <typename T>
QRSet<T>& QRSet<T>::intersection_update(const QRSet<T> &s) {
    auto it = this->begin();
    while (it != this->end()) {
        if (!s.has(*it))
            it = erase(*it);
        else
            it++;
    }
    return *this;
}

template <typename T>
QRSet<T>& QRSet<T>::intersection_update(const T &x) {
    bool flag = this->has(x);
    this->clear();
    if (flag)
        this->add(x);
    return *this;
}

template <typename T>
QRSet<T>& QRSet<T>::operator &=(const QRSet<T> &s) {
    return intersection_update(s);
}

template <typename T>
QRSet<T>& QRSet<T>::operator &=(const T &x) {
    return intersection_update(x);
}

template <typename T>
QRSet<T> operator &(const QRSet<T> &a, const QRSet<T> &b) {
    return a.intersection(b);
}

template <typename T>
QRSet<T> operator &(const QRSet<T> &a, const T &x) {
    return a.intersection(x);
}

template <typename T>
QRSet<T> operator &(const T &x, const QRSet<T> &a) {
    return a.intersection(x);
}


template <typename T>
QRSet<T> QRSet<T>::difference(const QRSet<T> &s) const {
    QRSet<T> res(*this);
    for (auto x: *this)
        if (s.has(x))
            res.erase(x);
    return res;
}

template <typename T>
QRSet<T> QRSet<T>::difference(const T &x) const {
    QRSet<T> res(*this);
    res.erase(x);
    return res;
}

template <typename T>
QRSet<T>& QRSet<T>::difference_update(const QRSet<T> &s) {
    auto it = this->begin();
    while (it != this->end()) {
        if (s.has(*it))
            it = erase(*it);
        else
            it++;
    }
    return *this;
}

template <typename T>
QRSet<T>& QRSet<T>::difference_update(const T &x) {
    this->erase(x);
    return *this;
}

template <typename T>
QRSet<T>& QRSet<T>::operator -=(const QRSet<T> &s) {
    return difference_update(s);
}

template <typename T>
QRSet<T>& QRSet<T>::operator -=(const T &x) {
    return difference_update(x);
}

template <typename T>
QRSet<T> operator -(const QRSet<T> &a, const QRSet<T> &b) {
    return a.difference(b);
}

template <typename T>
QRSet<T> operator -(const QRSet<T> &a, const T &x) {
    return a.difference(x);
}

template <typename T>
QRSet<T> operator -(const T &x, const QRSet<T> &a) {
    QRSet<T> res;
    if (!a.has(x))
        res.add(x);
    return res;
}


template <typename T>
QRSet<T> QRSet<T>::sym_difference(const QRSet<T> &s) const {
    QRSet<T> res;
    for (auto x: *this)
        if (!s.has(x))
            res.add(x);
    for (auto x: s)
        if (!this->has(x))
            res.add(x);
    return res;
}

template <typename T>
QRSet<T> QRSet<T>::sym_difference(const T &x) const {
    QRSet<T> res(*this);
    if (res.has(x))
        res.erase(x);
    else
        res.add(x);
    return res;
}

template <typename T>
QRSet<T>& QRSet<T>::sym_difference_update(const QRSet<T> &s) {
    for (auto x: s) {
        if (!has(x))
            add(x);
        else
            erase(x);
    }
    return *this;
}

template <typename T>
QRSet<T>& QRSet<T>::sym_difference_update(const T &x) {
    if (this->has(x))
        this->erase(x);
    else
        this->add(x);
    return *this;
}

template <typename T>
QRSet<T>& QRSet<T>::operator ^=(const QRSet<T> &s) {
    return sym_difference_update(s);
}

template <typename T>
QRSet<T>& QRSet<T>::operator ^=(const T &x) {
    return sym_difference_update(x);
}

template <typename T>
QRSet<T> operator ^(const QRSet<T> &a, const QRSet<T> &b) {
    return a.sym_difference(b);
}

template <typename T>
QRSet<T> operator ^(const QRSet<T> &a, const T &x) {
    return a.sym_difference(x);
}

template <typename T>
QRSet<T> operator ^(const T &x, const QRSet<T> &a) {
    return a.sym_difference(x);
}


template <typename T>
ostream& operator <<(ostream &os, const QRSet<T> &st) {
    os << "Set<";
    for (auto it = st.begin(); it != st.end();) {
        os << *it;
        it++;
        if (it != st.end()) os << ", ";
    }
    os << ">";
    return os;
}

#endif //OOP_SET_H