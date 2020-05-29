//
// Created by Георгий Куликов on 28.04.2020.
//

#include "ErrorAlloc.h"

ErrorAlloc::ErrorAlloc(const char* file, int line, const char* time, const char* msg) :
        VectorError(file, line, time, msg) {};

ErrorAlloc::~ErrorAlloc() {
    delete[] errorMsg;
}

const char* ErrorAlloc::what() const noexcept {
    return this->errorMsg;
}
