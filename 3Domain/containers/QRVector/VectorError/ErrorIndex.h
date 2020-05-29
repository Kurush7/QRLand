//
// Created by Георгий Куликов on 28.04.2020.
//

#ifndef LAB2_ERRORINDEX_H
#define LAB2_ERRORINDEX_H


#include "VectorError.h"

class ErrorIndex: public VectorError {
public:
    explicit ErrorIndex(const char* file, int line, const char* time, const char* msg);;
    virtual ~ErrorIndex();
    virtual const char* what() const noexcept override;
};


#endif //LAB2_ERRORINDEX_H
