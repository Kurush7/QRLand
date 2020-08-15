//
// Created by Георгий Куликов on 28.04.2020.
//

#include "ErrorIndex.h"


ErrorIndex::ErrorIndex(const char* file, int line, const char* time, const char* msg) :
        VectorError(file, line, time, msg) {};

ErrorIndex::~ErrorIndex() {
    delete[] errorMsg;
}

const char* ErrorIndex::what() const noexcept {
    return this->errorMsg;
}
