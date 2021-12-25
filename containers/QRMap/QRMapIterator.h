//
// Created by kurush on 29.05.2020.
//

#ifndef KG_QRMAPITERATOR_H
#define KG_QRMAPITERATOR_H

#endif //KG_QRMAPITERATOR_H

#include "../QRHashTable/QRHashTableIterator.h"
#include "../QRPair/QRPair.h"
#include "QRMapNode.h"

// todo this is const-iterator. simple one is needed
// todo for(auto x: map) is not supported
template<typename Key, typename Val>
class QRMapIterator: public QRHashTableIterator<Key> {
public:
    QRMapIterator(const QRHashTableIterator<Key> it): QRHashTableIterator<Key>(it) {}

    QRMapIterator<Key, Val>& operator++() {
        QRHashTableIterator<Key>::operator++();
        return *this;
    }

    const QRPair<Key, Val> unit() const {
        try {
            auto cN = reinterpret_cast<QRMapNode<Key, Val>*>(sptr<HashNode<Key>>(this->curNode).get());
            return makeQRPair(cN->key, cN->val);
        }
        catch (bad_weak_ptr &exc) {
            time_t t = time(NULL);
            throw QRHashTableBadIterator(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to get element value!");
        }
    }
    const Key key() const {
        try {
            auto cN = reinterpret_cast<QRMapNode<Key, Val>*>(sptr<HashNode<Key>>(this->curNode).get());
            return cN->key;
        }
        catch (bad_weak_ptr &exc) {
            time_t t = time(NULL);
            throw QRHashTableBadIterator(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to get element value!");
        }
    }
    const Val value() const {
        try {
            auto cN = reinterpret_cast<QRMapNode<Key, Val>*>(sptr<HashNode<Key>>(this->curNode).get());
            return cN->val;
        }
        catch (bad_weak_ptr &exc) {
            time_t t = time(NULL);
            throw QRHashTableBadIterator(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to get element value!");
        }
    }
};