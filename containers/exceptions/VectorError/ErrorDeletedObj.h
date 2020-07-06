//
// Created by Георгий Куликов on 03.05.2020.
//

#ifndef LAB2_ERRORDELETEDOBJ_H
#define LAB2_ERRORDELETEDOBJ_H


#include "VectorError.h"

class ErrorDeletedObj: public VectorError {
public:
    explicit ErrorDeletedObj(const char* file, int line, const char* time, const char* msg);;
    virtual ~ErrorDeletedObj();
    virtual const char* what() const noexcept override;
};


#endif //LAB2_ERRORDELETEDOBJ_H
