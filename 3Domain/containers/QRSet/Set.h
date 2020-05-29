//
// Created by kurush on 01.05.2020.
//

#ifndef OOP_SET_H
#define OOP_SET_H

#include <ctime>

#include "HashTable.h"
#include "HashTableIterator.h"
#include "SetError.h"
#include "SetBase.h"

const int DEFAULT_MAX_PRINT_SIZE = 8;

template <typename T>
class Set: public SetBase {
public:
    Set() = default;
    Set(const Set &);
    Set(Set &&) noexcept;
    Set(const std::initializer_list<T> &);
    Set(const T*, size_t);

    Set& operator=(const Set &);
    Set& operator=(Set &&) noexcept;
    Set& operator=(const std::initializer_list<T> &);

    virtual ~Set() = default;

    HashTableIterator<T> begin() const;
    HashTableIterator<T> end() const;

    T* make_array(); // todo const

    void clear() noexcept;
    HashTableIterator<T> add(const T &);
    HashTableIterator<T> erase(const T &);
    bool has(const T &) const noexcept;
    size_t size() const noexcept;
    bool isEmpty() const noexcept;

    Set join(const Set&) const;
    Set join(const T&) const;
    Set& join_update(const Set&);
    Set& join_update(const T&);
    Set& operator |=(const Set&);
    Set& operator |=(const T&);

    Set intersection(const Set&) const;
    Set intersection(const T&) const;
    Set& intersection_update(const Set&);
    Set& intersection_update(const T&);
    Set& operator &=(const Set&);
    Set& operator &=(const T&);

    Set difference(const Set&) const;
    Set difference(const T&) const;
    Set& difference_update(const Set&);
    Set& difference_update(const T&);
    Set& operator -=(const Set&);
    Set& operator -=(const T&);

    Set sym_difference(const Set&) const;
    Set sym_difference(const T&) const;
    Set& sym_difference_update(const Set&);
    Set& sym_difference_update(const T&);
    Set& operator ^=(const Set&);
    Set& operator ^=(const T&);

    bool isSubSet(const Set&) const;
    bool isSuperSet(const Set&) const;

    bool isEqual(const Set&) const;
    bool isNotEqual(const Set&) const;

private:
    HashTable<T> hashTable;
};

using namespace std;

template <typename T>
Set<T>::Set(const Set<T> &s): hashTable(s.hashTable) {}

template <typename T>
Set<T>::Set(Set<T> &&s) noexcept: hashTable(move(s.hashTable)) {}

template <typename T>
Set<T>::Set(const std::initializer_list<T> &list): Set() {
    for (auto el: list)
        hashTable.add(el);
}

template <typename T>
Set<T>::Set(const T* arr, size_t n): Set() {
    if (!arr && n) {
        time_t t = time(NULL);
        throw SetExceptionBadPointer(__FILE__, __LINE__, asctime(localtime(&t)),
                "bad pointer for array-constructor!");
    }

    for (size_t i = 0; i < n; ++i)
        hashTable.add(arr[i]);
}

template <typename T>
Set<T>& Set<T>::operator=(const Set &s) {
    hashTable = s.hashTable;
}

template <typename T>
Set<T>& Set<T>::operator=(Set &&s) noexcept {
    hashTable = move(s.hashTable);
}

template <typename T>
Set<T>& Set<T>::operator=(const initializer_list<T> &list) {
    hashTable.clear();
    for (auto el: list)
        hashTable.add(el);
}


template <typename T>
HashTableIterator<T> Set<T>::begin() const { return hashTable.begin(); }

template <typename T>
HashTableIterator<T> Set<T>::end() const { return hashTable.end(); }


template <typename T>
T* Set<T>::make_array() {
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
        throw SetExceptionBadAlloc(__FILE__, __LINE__, asctime(localtime(&t)), "Set failed to create an array!", size() * sizeof(T));
    }
}


template <typename T>
void Set<T>::clear() noexcept { hashTable.clear(); }

template <typename T>
HashTableIterator<T> Set<T>::add(const T &x) { return hashTable.add(x); }

template <typename T>
HashTableIterator<T> Set<T>::erase(const T &x) { return hashTable.erase(x); }

template <typename T>
bool Set<T>::has(const T &x) const noexcept { return hashTable.has(x); }

template <typename T>
size_t Set<T>::size() const noexcept { return hashTable.size(); }

template <typename T>
bool Set<T>::isEmpty() const noexcept { return hashTable.size() == 0; }


template <typename T>
bool Set<T>::isEqual(const Set<T> &a) const {
    if (size() != a.size())
        return false;

    bool flag = true;
    for (auto it = begin(); it != end() && flag; ++it)
        flag &= a.has(*it);

    return flag;
}

template <typename T>
bool operator ==(const Set<T> &a, const Set<T> &b) {
    return a.isEqual(b);
}

template <typename T>
bool Set<T>::isNotEqual(const Set<T> &a) const {
    return !(isEqual(a));
}

template <typename T>
bool operator !=(const Set<T> &a, const Set<T> &b) {
    return a.isNotEqual(b);
}


template <typename T>
bool Set<T>::isSubSet(const Set<T> &s) const {
    bool flag = true;
    for (auto it = this->begin(); it != this->end() && flag; ++it)
        if (!s.has(*it))
            flag = false;
    return flag;
}

template <typename  T>
bool operator <=(const Set<T> &a, const Set<T> &b) {
    return a.isSubSet(b);
}

template <typename T>
bool Set<T>::isSuperSet(const Set<T> &s) const {
    bool flag = true;
    for (auto x: s)
        if (!has(x))
            flag = false;
    return flag;
}

template <typename  T>
bool operator >=(const Set<T> &a, const Set<T> &b) {
    return a.isSuperSet(b);
}


template <typename T>
Set<T> Set<T>::join(const Set<T> &s) const {
    Set<T> res(*this);
    for (auto x: s)
        res.add(x);
    return res;
}

template <typename T>
Set<T> Set<T>::join(const T &x) const {
    Set<T> res(*this);
    res.add(x);
    return res;
}

template <typename T>
Set<T>& Set<T>::join_update(const Set<T> &s) {
    for (auto x: s)
        if (!has(x))
            add(x);
    return *this;
}

template <typename T>
Set<T>& Set<T>::join_update(const T &x) {
    add(x);
    return *this;
}

template <typename T>
Set<T>& Set<T>::operator |=(const Set<T> &s) {
    return join_update(s);
}

template <typename T>
Set<T>& Set<T>::operator |=(const T &x) {
    return join_update(x);
}

template <typename T>
Set<T> operator |(const Set<T> &a, const Set<T> &b) {
    return a.join(b);
}

template <typename T>
Set<T> operator |(const T &x, const Set<T> &a) {
    return a.join(x);
}

template <typename T>
Set<T> operator |(const Set<T> &a, const T &x) {
    return a.join(x);
}


template <typename T>
Set<T> Set<T>::intersection(const Set &s) const {
    Set<T> res(*this);
    for (auto x: *this)
        if (!s.has(x))
            res.erase(x);
    return res;
}

template <typename T>
Set<T> Set<T>::intersection(const T &x) const {
    Set<T> res;
    if (this->has(x))
        res.add(x);
    return res;
}

template <typename T>
Set<T>& Set<T>::intersection_update(const Set<T> &s) {
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
Set<T>& Set<T>::intersection_update(const T &x) {
    bool flag = this->has(x);
    this->clear();
    if (flag)
        this->add(x);
    return *this;
}

template <typename T>
Set<T>& Set<T>::operator &=(const Set<T> &s) {
    return intersection_update(s);
}

template <typename T>
Set<T>& Set<T>::operator &=(const T &x) {
    return intersection_update(x);
}

template <typename T>
Set<T> operator &(const Set<T> &a, const Set<T> &b) {
    return a.intersection(b);
}

template <typename T>
Set<T> operator &(const Set<T> &a, const T &x) {
    return a.intersection(x);
}

template <typename T>
Set<T> operator &(const T &x, const Set<T> &a) {
    return a.intersection(x);
}


template <typename T>
Set<T> Set<T>::difference(const Set<T> &s) const {
    Set<T> res(*this);
    for (auto x: *this)
        if (s.has(x))
            res.erase(x);
    return res;
}

template <typename T>
Set<T> Set<T>::difference(const T &x) const {
    Set<T> res(*this);
    res.erase(x);
    return res;
}

template <typename T>
Set<T>& Set<T>::difference_update(const Set<T> &s) {
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
Set<T>& Set<T>::difference_update(const T &x) {
    this->erase(x);
    return *this;
}

template <typename T>
Set<T>& Set<T>::operator -=(const Set<T> &s) {
    return difference_update(s);
}

template <typename T>
Set<T>& Set<T>::operator -=(const T &x) {
    return difference_update(x);
}

template <typename T>
Set<T> operator -(const Set<T> &a, const Set<T> &b) {
    return a.difference(b);
}

template <typename T>
Set<T> operator -(const Set<T> &a, const T &x) {
    return a.difference(x);
}

template <typename T>
Set<T> operator -(const T &x, const Set<T> &a) {
    Set<T> res;
    if (!a.has(x))
        res.add(x);
    return res;
}


template <typename T>
Set<T> Set<T>::sym_difference(const Set<T> &s) const {
    Set<T> res;
    for (auto x: *this)
        if (!s.has(x))
            res.add(x);
    for (auto x: s)
        if (!this->has(x))
            res.add(x);
    return res;
}

template <typename T>
Set<T> Set<T>::sym_difference(const T &x) const {
    Set<T> res(*this);
    if (res.has(x))
        res.erase(x);
    else
        res.add(x);
    return res;
}

template <typename T>
Set<T>& Set<T>::sym_difference_update(const Set<T> &s) {
    for (auto x: s) {
        if (!has(x))
            add(x);
        else
            erase(x);
    }
    return *this;
}

template <typename T>
Set<T>& Set<T>::sym_difference_update(const T &x) {
    if (this->has(x))
        this->erase(x);
    else
        this->add(x);
    return *this;
}

template <typename T>
Set<T>& Set<T>::operator ^=(const Set<T> &s) {
    return sym_difference_update(s);
}

template <typename T>
Set<T>& Set<T>::operator ^=(const T &x) {
    return sym_difference_update(x);
}

template <typename T>
Set<T> operator ^(const Set<T> &a, const Set<T> &b) {
    return a.sym_difference(b);
}

template <typename T>
Set<T> operator ^(const Set<T> &a, const T &x) {
    return a.sym_difference(x);
}

template <typename T>
Set<T> operator ^(const T &x, const Set<T> &a) {
    return a.sym_difference(x);
}


template <typename T>
ostream& operator <<(ostream &os, const Set<T> &st) {
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