//
// Created by Георгий Куликов on 07.05.2020.
//

#include "ErrorZeroDivision.h"

ErrorZeroDivision::ErrorZeroDivision(const char* file, int line, const char* time, const char* msg) :
        VectorError(file, line, time, msg) {};

ErrorZeroDivision::~ErrorZeroDivision() {
    delete[] errorMsg;
}

const char* ErrorZeroDivision::what() const noexcept {
    return this->errorMsg;
}