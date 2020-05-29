//
// Created by kurush on 29.05.2020.
//

#ifndef KG_QRMAPNODE_H
#define KG_QRMAPNODE_H

#include "../QRHashTable/HashNode.h"

template <typename Key, typename Val>
class QRMapNode: public HashNode<Key> {
public:
    QRMapNode(const Key &k, const Val &v): HashNode<Key>(k), val(v) {}
    QRMapNode(const Key &k, const Val &v, const shared_ptr<HashNode<Key>> next): HashNode<Key>(k, next), val(v) {}

    virtual std::shared_ptr<HashNode<Key>> clone() const override {
        auto p = new QRMapNode<Key, Val>(this->key, val, this->next);
        return std::shared_ptr<HashNode<Key>>(p);
    }

    Val val;
};


#endif //KG_QRMAPNODE_H
