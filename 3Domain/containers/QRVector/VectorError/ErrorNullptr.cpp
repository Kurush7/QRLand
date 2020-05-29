//
// Created by Георгий Куликов on 06.05.2020.
//

#include "ErrorNullptr.h"

ErrorNullptr::ErrorNullptr(const char* file, int line, const char* time, const char* msg) :
        VectorError(file, line, time, msg) {};

ErrorNullptr::~ErrorNullptr() {
    delete[] errorMsg;
}

const char* ErrorNullptr::what() const noexcept {
    return this->errorMsg;
}