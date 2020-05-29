//
// Created by kurush on 28.04.2020.
//

#ifndef OOP_HASHNODE_H
#define OOP_HASHNODE_H

#include <memory>

template <typename T>
class HashNode
{
public:
    HashNode() = default;
    HashNode(const T &, const std::shared_ptr<HashNode<T>>);
    HashNode(const T &);
    HashNode(const HashNode &);
    HashNode(HashNode &&) noexcept;

    virtual std::shared_ptr<HashNode> clone() const;

    ~HashNode() = default;

    HashNode& operator=(const HashNode &);
    HashNode& operator=(HashNode &&) noexcept;

    bool operator==(const HashNode &node) const;
    bool operator!=(const HashNode &node) const;

    T key;
    std::shared_ptr<HashNode<T>> next = nullptr;
};

using namespace std;

template <typename T>
shared_ptr<HashNode<T>> HashNode<T>::clone() const {
    return shared_ptr<HashNode<T>>(new HashNode<T>(key, next));
}

template <typename T>
HashNode<T>::HashNode(const T &key, const shared_ptr<HashNode<T>> next): key(key), next(next) {}

template <typename T>
HashNode<T>::HashNode(const T &key): key(key), next(nullptr) {}

template <typename T>
HashNode<T>::HashNode(const HashNode &node): key(node.key), next(node.next) {}

template <typename T>
HashNode<T>::HashNode(HashNode &&node) noexcept: next(node.next)  {
    key = move(node.key);
}

template <typename T>
HashNode<T>& HashNode<T>::operator=(const HashNode<T> &node) {
    if(this == &node) return *this;
    key = node.key;
    next = node.next;
    return *this;
}

template <typename T>
HashNode<T>& HashNode<T>::operator=(HashNode<T> &&node) noexcept {
    key = move(node.key);
    next = node.next;
    return *this;
}

template <typename T>
bool HashNode<T>::operator==(const HashNode &node) const {
    return key == node.key && next == node.next;
}

template <typename T>
bool HashNode<T>::operator!=(const HashNode &node) const {
    return !(node == *this);
}

#endif // OOP_HASHNODE_H