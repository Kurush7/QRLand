//
// Created by Георгий Куликов on 07.05.2020.
//

#include "ErrorSize.h"

ErrorSize::ErrorSize(const char* file, int line, const char* time, const char* msg) :
        VectorError(file, line, time, msg) {};

ErrorSize::~ErrorSize() {
    delete[] errorMsg;
}

const char* ErrorSize::what() const noexcept {
    return this->errorMsg;
}