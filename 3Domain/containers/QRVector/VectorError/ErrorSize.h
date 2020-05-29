//
// Created by Георгий Куликов on 07.05.2020.
//

#ifndef LAB2_ERRORSIZE_H
#define LAB2_ERRORSIZE_H

#include "VectorError.h"

class ErrorSize: public VectorError {
public:
    explicit ErrorSize(const char* file, int line, const char* time, const char* msg);
    virtual ~ErrorSize();
    virtual const char* what() const noexcept override;
};


#endif //LAB2_ERRORSIZE_H
