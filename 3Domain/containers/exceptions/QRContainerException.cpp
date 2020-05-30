//
// Created by kurush on 29.05.2020.
//

#include "QRContainerException.h"


using namespace std;

QRBadPointerException::QRBadPointerException(const char *file, int line, const char *time,
                                             const char *msg): QRContainerException(file, line, time, msg) {}
const char* QRBadPointerException::what() const noexcept {
    try {
        const char *buf = "<< Container Bad Pointer Exception!\n";
        size_t len = strlen(errorMsg) + strlen(buf) + 16;

        char *msg = new char[len + 1];

        sprintf(msg, "%s\n%s>>", buf, errorMsg);

        delete[] errorMsg;
        const_cast<QRBadPointerException *>(this)->errorMsg = msg;

        return errorMsg;
    }
    catch (bad_alloc &exc) {
        throw;
    }
}