//
// Created by kurush on 29.04.2020.
//

#ifndef OOP_HASHTABLEBASE_H
#define OOP_HASHTABLEBASE_H

#include <iostream>
#include <memory>

#include "../../QRDefines.h"

class HashTableBase {
public:
    HashTableBase();
    explicit HashTableBase(size_t);
    HashTableBase(size_t, size_t);
    virtual ~HashTableBase() = 0;

    explicit virtual operator bool() const;
    size_t size() const;

protected:
    static const size_t DEFAULT_SIZE = 4;
    static constexpr float MAX_ALPHA = 3.0;

    sptr<size_t> capacity;
    size_t curSize;
};



#endif //OOP_HASHTABLEBASE_H
