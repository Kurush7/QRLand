//
// Created by Георгий Куликов on 03.05.2020.
//

#include "ErrorDeletedObj.h"

ErrorDeletedObj::ErrorDeletedObj(const char* file, int line, const char* time, const char* msg) :
        VectorError(file, line, time, msg) {};

ErrorDeletedObj::~ErrorDeletedObj() {
    delete[] errorMsg;
}

const char* ErrorDeletedObj::what() const noexcept {
    return this->errorMsg;
}