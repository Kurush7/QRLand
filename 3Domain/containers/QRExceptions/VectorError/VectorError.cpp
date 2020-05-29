//
// Created by Георгий Куликов on 28.04.2020.
//

#include "VectorError.h"

VectorError::VectorError(const char* file, int line, const char* time, const char* msg)
:QRContainerException(file, line, time, msg) {
    this->errorMsg = new char[strlen(file) + 5 + strlen(time) + strlen(msg) + 1];
    sprintf(this->errorMsg, "file: %s, line: %-3d, time: %s, error: %s", file, line, time, msg);
}

VectorError::~VectorError() {
    delete[] errorMsg;
}

const char* VectorError::what() const noexcept {
    return this->errorMsg;
}
