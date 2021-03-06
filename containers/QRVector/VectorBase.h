//
// Created by Георгий Куликов on 29.04.2020.
//

#ifndef LAB2_VECTORBASE_H
#define LAB2_VECTORBASE_H


#include <cstdio>
#include <memory>
#include "../exceptions/VectorError/Errors.h"

class VectorBase {
public:
    explicit VectorBase(size_t sz = 0);
    virtual ~VectorBase() = 0;

    virtual size_t getSize() const;
    virtual void setSize(size_t sz) const;

    virtual bool isEmpty() const;
    operator bool() const;
protected:
    sptr<size_t> size;
    size_t max_size;
};


#endif //LAB2_VECTORBASE_H
