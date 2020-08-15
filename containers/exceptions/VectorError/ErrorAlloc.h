//
// Created by Георгий Куликов on 28.04.2020.
//

#ifndef LAB2_ERRORALLOC_H
#define LAB2_ERRORALLOC_H


#include "VectorError.h"

class ErrorAlloc: public VectorError {
public:
    explicit ErrorAlloc(const char* file, int line, const char* time, const char* msg);;
    virtual ~ErrorAlloc();
    virtual const char* what() const noexcept override;
};


#endif //LAB2_ERRORALLOC_H
