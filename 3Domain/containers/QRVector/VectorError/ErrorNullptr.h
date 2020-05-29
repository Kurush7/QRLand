//
// Created by Георгий Куликов on 06.05.2020.
//

#ifndef LAB2_ERRORNULLPTR_H
#define LAB2_ERRORNULLPTR_H

#include "VectorError.h"

class ErrorNullptr: public VectorError {
public:
    explicit ErrorNullptr(const char* file, int line, const char* time, const char* msg);;
    virtual ~ErrorNullptr();
    virtual const char* what() const noexcept override;
};


#endif //LAB2_ERRORNULLPTR_H
