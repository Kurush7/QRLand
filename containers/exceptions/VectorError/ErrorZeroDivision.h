//
// Created by Георгий Куликов on 07.05.2020.
//

#ifndef LAB2_ERRORZERODIVISION_H
#define LAB2_ERRORZERODIVISION_H

#include "VectorError.h"

class ErrorZeroDivision: public VectorError {
public:
    explicit ErrorZeroDivision(const char* file, int line, const char* time, const char* msg);
    virtual ~ErrorZeroDivision();
    virtual const char* what() const noexcept override;
};


#endif //LAB2_ERRORZERODIVISION_H
