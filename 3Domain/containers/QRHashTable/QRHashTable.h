//
// Created by kurush on 28.04.2020.
//

#ifndef OOP_HASHTABLE_H
#define OOP_HASHTABLE_H

#include <initializer_list>
#include <iostream>
#include <ostream>
#include <sstream>
#include <typeinfo>

#include "HashNode.h"
#include "HashTableBase.h"
#include "../QRExceptions/HashTableError.h"

template <typename T>
class QRHashTableIterator;

template <typename T>
class QRHashTable: public HashTableBase
{
public:
    QRHashTable();
    QRHashTable(const QRHashTable &);
    QRHashTable(QRHashTable &&) noexcept;
    QRHashTable(const std::initializer_list<T> &list);

    QRHashTable& operator=(const QRHashTable &);
    QRHashTable& operator=(QRHashTable &&) noexcept = default;

    virtual ~QRHashTable() = default;

    QRHashTableIterator<T> begin() const;
    QRHashTableIterator<T> end() const;

    void clear() noexcept;
    QRHashTableIterator<T> add(const T &);    // returns iterator of added element
    void add(const std::shared_ptr<HashNode<T>>);
    QRHashTableIterator<T> erase(const T &);  // returns iterator of next element or nullptr
    const std::shared_ptr<HashNode<T>> getNode(const T&);
    bool has(const T &) const noexcept;


private:
    std::shared_ptr<std::shared_ptr<HashNode<T>>[]> createEmptyTable(size_t);
    void copyTable(const QRHashTable &);
    void moveTable(QRHashTable &);
    void grow() noexcept;

    size_t makeHash(const T&, size_t) const;
    size_t getFitSize(size_t);

    std::shared_ptr<std::shared_ptr<HashNode<T>>[]> table = nullptr;
};


using namespace std;

template <typename T>
QRHashTable<T>::QRHashTable(): HashTableBase(DEFAULT_SIZE) {
    try {
        table = createEmptyTable(DEFAULT_SIZE);
    }
    catch (QRBadAllocException &exc) {
        cerr << "HashTable default constructor error: " << exc.what() << '\n';
        throw;
    }
}

template <typename T>
QRHashTable<T>::QRHashTable(const QRHashTable<T> &table) {
    try {
        copyTable(table);
    }
    catch (QRBadAllocException &exc) {
        cerr << "HashTable copy constructor error: " << exc.what() << '\n';
        throw;
    }
}

template <typename T>
QRHashTable<T>::QRHashTable(QRHashTable<T> &&table) noexcept {
    moveTable(table);
}

template <typename T>
QRHashTable<T>::QRHashTable(const std::initializer_list<T> &list) {
    try {
        *capacity = getFitSize(list.size());

        table = createEmptyTable(*capacity);
        for (auto el: list) {
            add(el);
        }
    }
    catch (exception &exc) {
        cerr << "HashTable initList constructor error: " << exc.what() << '\n';
        throw;
    }
}

template <typename T>
QRHashTable<T>& QRHashTable<T>::operator=(const QRHashTable<T> &other) {
    if (this == &other)
        return *this;

    try {
        copyTable(other);
    }
    catch (QRBadAllocException &exc) {
        cerr << "HashTable operator= copy error: " << exc.what() << '\n';
        throw;
    }
}


template <typename T>
QRHashTableIterator<T> QRHashTable<T>::add(const T &key) {
    try {
        if (curSize > *capacity * MAX_ALPHA)
            grow();

        size_t hash = makeHash(key,*capacity);
        auto node = table[hash];
        while (node != nullptr && !(node->key == key))
            node = node->next;

        if (node != nullptr) {
            return end();
        }

        auto x = shared_ptr<HashNode<T>>(new HashNode<T>(key, table[hash]));
        table[hash] = x;
        curSize++;

        return QRHashTableIterator<T>(table, capacity, hash, table[hash]);
    }
    catch(const std::bad_alloc &exc) {
        time_t t = time(nullptr);
        throw QRBadAllocException(__FILE__, __LINE__, asctime(localtime(&t)),
                "Failed to add element!", size() * sizeof(T));
    }
}

template <typename T>
void QRHashTable<T>::add(const std::shared_ptr<HashNode<T>> nd) {
    try {
        if (curSize > *capacity * MAX_ALPHA)
            grow();

        size_t hash = makeHash(nd->key,*capacity);
        auto node = table[hash];
        while (node != nullptr && !(node->key == nd->key))
            node = node->next;

        if (node != nullptr) {
            return;
        }

        auto x = shared_ptr<HashNode<T>>(nd->clone());
        x->next = table[hash];
        table[hash] = x;
        curSize++;
    }
    catch(const std::bad_alloc &exc) {
        time_t t = time(nullptr);
        throw QRBadAllocException(__FILE__, __LINE__, asctime(localtime(&t)),
                                  "Failed to add element!", size() * sizeof(T));
    }
}

template <typename T>
QRHashTableIterator<T> QRHashTable<T>::erase(const T &key) {
    size_t hash = makeHash(key, *capacity);

    shared_ptr<HashNode<T>> node = table[hash], prevNode;
    while (node != nullptr && !(node->key == key)) {
        prevNode = node;
        node = node->next;
    }

    if (node == nullptr){
        return end();
    }

    if (prevNode == nullptr)
        table[hash] = node->next;
    else
        prevNode->next = node->next;
    curSize--;

    if (node->next)     // find next node for iterator
        node = node->next;
    else if (hash < *capacity-1)
        node = table[++hash];
    else
        node = nullptr, ++hash;
    return QRHashTableIterator<T>(table, capacity, hash, node);
}

template <typename T>
bool QRHashTable<T>::has(const T &key) const noexcept {
    size_t hash = makeHash(key, *capacity);

    auto node = table[hash];
    while (node != nullptr && !(node->key == key))
        node = node->next;

    return node != nullptr;
}

template <typename T>
const std::shared_ptr<HashNode<T>> QRHashTable<T>::getNode(const T &key) {
    size_t hash = makeHash(key, *capacity);

    auto node = table[hash];
    while (node != nullptr && !(node->key == key))
        node = node->next;

    return node;
}


template <typename T>
void QRHashTable<T>::clear() noexcept {
    for (size_t i = 0; i < *capacity; ++i)
        table[i] = nullptr;
    curSize = 0;
}


template <typename T>
shared_ptr<shared_ptr<HashNode<T>>[]> QRHashTable<T>::createEmptyTable(size_t sz) {
    try {
        shared_ptr<shared_ptr<HashNode<T>>[]> newTable(new shared_ptr<HashNode<T>>[sz]{nullptr});
        return newTable;
    }
    catch(const std::bad_alloc &exc) {
        time_t t = time(nullptr);
        throw QRBadAllocException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to allocate new table!", *capacity);
    }
}

template <typename T>
void QRHashTable<T>::copyTable(const QRHashTable &copyTable) {
    try {
        auto newTable = createEmptyTable(*copyTable.capacity);
        shared_ptr<HashNode<T>> head, node, copyHead;

        for (size_t i = 0; i < *copyTable.capacity; ++i) {
            copyHead = copyTable.table[i];
            if (!copyHead)
                continue;

            head.reset(new HashNode<T>(copyHead->key, nullptr));
            newTable[i] = head;
            copyHead = copyHead->next;
            while (copyHead) {
                node.reset(new HashNode<T>(copyHead->key, nullptr));
                head->next = node;
                head = head->next;
                copyHead = copyHead->next;
            }
        }
        table = newTable;
        curSize = copyTable.curSize;
        capacity = copyTable.capacity;
    }
    catch (QRBadAllocException &exc) {
        throw;
    }
    catch(const std::bad_alloc &exc) {
        throw;
    }
}

template <typename T>
void QRHashTable<T>::moveTable(QRHashTable &moveTable) {
    table = moveTable.table;
    capacity = moveTable.capacity;
    curSize = moveTable.curSize;
}

template <typename T>
void QRHashTable<T>::grow() noexcept {
    try {
        size_t newCapacity = *capacity * 2;
        auto newTable = createEmptyTable(newCapacity);

        shared_ptr<HashNode<T>> node, next;
        for (size_t i = 0; i < *capacity; i++) {
            node = table[i];
            while (node != nullptr) {
                size_t hash = makeHash(node->key, newCapacity);
                next = node->next;
                node->next = newTable[hash];
                newTable[hash] = node;
                node = next;
            }
        }

        table = newTable;
        *capacity = newCapacity;
    }
    catch (QRBadAllocException &exc) {
        cerr<< "HashTable grow failed: " << exc.what()<<endl;
    }
}


template <typename T>
QRHashTableIterator<T> QRHashTable<T>::begin() const { return QRHashTableIterator<T>(table, capacity, false); }

template <typename T>
QRHashTableIterator<T> QRHashTable<T>::end() const { return QRHashTableIterator<T>(table, capacity, true);	}


template <typename T>
size_t QRHashTable<T>::makeHash(const T &key, size_t capacity) const {
    size_t hash = 0;
    size_t len = sizeof(key);

    char s[len];
    memcpy(s, &key, len);

    for (size_t i = 0; i < len; i++)
    {
        hash = (hash * 2017 + s[i] * 17) % capacity;
    }
    return hash;
}

template <typename T>
size_t QRHashTable<T>::getFitSize(size_t required) {
    size_t sz = DEFAULT_SIZE;
    while (sz * MAX_ALPHA < required)
        sz *= 2;
    return sz;
}


template <typename T>
ostream& operator<<(ostream &os, const QRHashTable<T> &table) {
    os<<"HashTable<" << table.size() << " elements>";
    return os;
}

#endif // OOP_HASHTABLE_H