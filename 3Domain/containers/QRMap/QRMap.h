//
// Created by kurush on 29.05.2020.
//

#ifndef KG_QRMAP_H
#define KG_QRMAP_H


#include "../QRHashTable/QRHashTable.h"
#include "../exceptions/SetError.h"
#include "QRMapBase.h"
#include "QRMapIterator.h"
#include "QRMapNode.h"

template <typename Key, typename Val>
class QRMap: public QRMapBase {
public:
    QRMap() = default;
    virtual ~QRMap() = default;

    QRMapIterator<Key, Val> begin() const {return QRMapIterator<Key, Val>(hashTable.begin());}
    QRMapIterator<Key, Val> end() const {return QRMapIterator<Key, Val>(hashTable.end());}

    Val& operator [](const Key &k) {
        QRMapNode<Key, Val>* nd = reinterpret_cast<QRMapNode<Key, Val>*>(hashTable.getNode(k).get());
        return nd->val;
    }

    void clear() noexcept;
    void add(const Key &, const Val &);
    QRHashTableIterator<Key> erase(const Key &key) {return hashTable.erase(key);};
    bool has(const Key &key) const noexcept {return hashTable.has(key);}
    size_t size() const noexcept {return hashTable.size();}
    bool isEmpty() const noexcept {return hashTable.size() == 0;}

private:
    QRHashTable<Key> hashTable;
};

using namespace std;


template <typename Key, typename Val>
void QRMap<Key, Val>::add(const Key &k, const Val &v) {
    auto p = std::shared_ptr<HashNode<Key>>(new QRMapNode<Key, Val>(k, v));
    hashTable.add(p);
}


#endif //KG_QRMAP_H
