//
// Created by kurush on 29.05.2020.
//

#include "QRContainerException.h"

using namespace std;

QRContainerException::QRContainerException(const char *file, int line, const char *time, const char *msg) {
    try {
        int n = strlen(time);
        bool flag = false;
        if (time[n - 1] == '\n')
            flag = true;

        size_t len = strlen(file) + 64 + n + strlen(msg) + 1;
        errorMsg = new char[len];
        errorMsg[0] = '\0';
        strcat(errorMsg, msg);
        sprintf(errorMsg, "  file: %s\n  line: %-8d\n  time: %s%s  error: %s\n", file, line, time, flag ? "" : "\n",
                msg);
    }
    catch (bad_alloc &exc) {
        throw;
    }
}
QRContainerException::~QRContainerException() {
    delete[] errorMsg;
}
const char* QRContainerException::what() const noexcept {
    return errorMsg;
}


QRBadAllocException::QRBadAllocException(const char *file, int line, const char *time,
                                           const char *msg, size_t size): QRContainerException(file, line, time, msg),
                                                                          size(size) {}
const char* QRBadAllocException::what() const noexcept {
    try {
        const char *buf = "<< Container Bad Alloc Exception!\n  memory amount requested: ";
        size_t len = strlen(errorMsg) + strlen(buf) + 16;

        char *msg = new char[len + 1];

        sprintf(msg, "%s%-8zd\n%s>>", buf, size, errorMsg);

        delete[] errorMsg;
        const_cast<QRBadAllocException *>(this)->errorMsg = msg;

        return errorMsg;
    }
    catch (bad_alloc &exc) {
        throw;
    }
}