//
// Created by kurush on 28.04.2020.
//

#include "SetError.h"

using namespace std;

QRSetException::QRSetException(const char *file, int line, const char *time, const char *msg)
: QRContainerException(file, line, time, msg) {
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
QRSetException::~QRSetException() {
    delete[] errorMsg;
}
const char* QRSetException::what() const noexcept {
    return errorMsg;
}


QRSetExceptionBadPointer::QRSetExceptionBadPointer(const char *file, int line, const char *time,
                                                   const char *msg): QRSetException(file, line, time, msg) {}
const char* QRSetExceptionBadPointer::what() const noexcept {
    try {
        const char *buf = "<< Set Bad Pointer Exception!\n";
        size_t len = strlen(errorMsg) + strlen(buf) + 16;

        char *msg = new char[len + 1];

        sprintf(msg, "%s%s>>", buf, errorMsg);

        delete[] errorMsg;
        const_cast<QRSetExceptionBadPointer *>(this)->errorMsg = msg;

        return errorMsg;
    }
    catch (bad_alloc &exc) {
        throw;
    }
}