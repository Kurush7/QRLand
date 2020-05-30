//
// Created by kurush on 30.04.2020.
//

#ifndef OOP_HASHTABLEITERATOR_H
#define OOP_HASHTABLEITERATOR_H

#include <iostream>
#include <iterator>

#include "QRHashTable.h"

template <typename T>
class QRHashTableIterator : public std::iterator<std::output_iterator_tag, T>
{
private:
    friend class QRHashTable<T>;
    QRHashTableIterator(const std::shared_ptr<std::shared_ptr<HashNode<T>>[]> &,
                        const std::shared_ptr<size_t> &, bool is_end = false);
    QRHashTableIterator(const std::shared_ptr<std::shared_ptr<HashNode<T>>[]> &,
                        const std::shared_ptr<size_t> &, size_t, const std::shared_ptr<HashNode<T>>&);
public:
    QRHashTableIterator(const QRHashTableIterator &) = default;
    ~QRHashTableIterator() = default;

    virtual bool operator!=(QRHashTableIterator const&) const;
    virtual bool operator==(QRHashTableIterator const&) const;

    virtual const T& operator*() const;
    virtual const T* operator->() const;

    virtual explicit operator bool() const;

    virtual QRHashTableIterator<T>& operator++();
    virtual QRHashTableIterator<T> operator++(int);

protected:
    std::weak_ptr<std::shared_ptr<HashNode<T>>[]> table;
    std::weak_ptr<size_t> capacity;
    size_t tableIndex = 0;
    std::weak_ptr<HashNode<T>> curNode;
private:
    bool setNextValid();
};

template <typename T>
QRHashTableIterator<T>::QRHashTableIterator(const  shared_ptr<shared_ptr<HashNode<T>>[]>  &t,
                                            const shared_ptr<size_t> &capacity, bool is_end)
: table(t), capacity(capacity) {
    tableIndex = is_end? *capacity: 0;
    if (!is_end) {
        curNode = t[0];
        HashNode<T>* cN = t[0].get();
        if (cN == nullptr)
            setNextValid();
    }
}

template <typename T>
QRHashTableIterator<T>::QRHashTableIterator(const std::shared_ptr<std::shared_ptr<HashNode<T>>[]> &t,
                                            const std::shared_ptr<size_t> &capacity, size_t tabInd,
                                            const std::shared_ptr<HashNode<T>> &cN)
: table(t), capacity(capacity), tableIndex(tabInd) {
    if (cN)
        curNode = weak_ptr<HashNode<T>>(cN);
    else
        setNextValid();
}


template <typename T>
bool QRHashTableIterator<T>::operator==(QRHashTableIterator const &it) const {
    return (curNode.lock() == it.curNode.lock() &&
            tableIndex == it.tableIndex);
}

template <typename T>
bool QRHashTableIterator<T>::operator!=(QRHashTableIterator const &it) const {
    return !(*this == it);
}


template <typename T>
QRHashTableIterator<T>::operator bool() const {
    try {
        auto n = shared_ptr<size_t>(capacity);
        return tableIndex < *n;
    }
    catch (bad_weak_ptr &exc) {
        time_t t = time(NULL);
        throw QRHashTableBadIterator(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to bool() iterator!");
    }
}


template <typename T>
const T& QRHashTableIterator<T>::operator*() const {
    try {
        auto cN = shared_ptr<HashNode<T>>(curNode);
        return cN->key;
    }
    catch (bad_weak_ptr &exc) {
        time_t t = time(NULL);
        throw QRHashTableBadIterator(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to get element value!");
    }
}

template <typename T>
const T* QRHashTableIterator<T>::operator->() const {
    try {
        auto cN = shared_ptr<HashNode<T>>(curNode);
        return &cN->key;
    }
    catch (bad_weak_ptr &exc) {
        time_t t = time(NULL);
        throw QRHashTableBadIterator(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to get element pointer!");
    }
}

template <typename T>
bool QRHashTableIterator<T>::setNextValid() {
    try {
        auto n = shared_ptr<size_t>(capacity);
        if (tableIndex == *n)
            return true;

        auto cN = curNode.lock();
        auto t = shared_ptr<std::shared_ptr<HashNode<T>>[]>(table);
        if (tableIndex < *n) {
            if (cN && cN->next) {
                cN = cN->next;
            } else {
                if (cN && !cN->next)
                    cN = ++tableIndex < *n ? t[tableIndex] : nullptr;
                while (!cN && tableIndex < *n)
                    cN = ++tableIndex < *n ? t[tableIndex] : nullptr;
            }
        }
        curNode = weak_ptr<HashNode<T>>(cN);
        return tableIndex == *n;
    }
    catch (bad_weak_ptr &exc) {
        time_t t = time(NULL);
        throw QRHashTableBadIterator(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to get next element!");
    }
}

template <typename T>
QRHashTableIterator<T>& QRHashTableIterator<T>::operator++()
{
    try {
        setNextValid();
        return *this;
    }
    catch (QRHashTableBadIterator &exc) {
        auto n = capacity.lock();
        if (!n) {
            time_t t = time(NULL);
            throw QRHashTableBadIterator(__FILE__, __LINE__, asctime(localtime(&t)),
                                         "Failed to get HashTable size while incrementing!");
        }
        curNode = weak_ptr<HashNode<T>>();
        tableIndex = *n;
    }
}

template <typename T>
QRHashTableIterator<T> QRHashTableIterator<T>::operator++(int)
{
    auto it(*this);
    ++(*this);
    return it;
}

#endif //OOP_HASHTABLEITERATOR_H