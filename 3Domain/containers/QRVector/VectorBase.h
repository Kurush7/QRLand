//
// Created by Георгий Куликов on 29.04.2020.
//

#ifndef LAB2_VECTORBASE_H
#define LAB2_VECTORBASE_H


#include <cstdio>
#include <memory>
#include "../QRExceptions/VectorError/Errors.h"

class VectorBase {
public:
    explicit VectorBase(size_t sz = 0);
    virtual ~VectorBase() = 0;

    virtual size_t getSize() const;

    virtual bool isEmpty() const;
    operator bool() const;
protected:
    std::shared_ptr<size_t> size;
    size_t max_size;
};


#endif //LAB2_VECTORBASE_H
