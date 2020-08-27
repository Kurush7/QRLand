//
// Created by kurush on 29.05.2020.
//

#include "QRException.h"

using namespace std;

QRException::QRException(const char *file, int line, const char *time, const char *msg) {
    try {
        int n = strlen(time);
        bool flag = false;
        if (time[n - 1] == '\n')
            flag = true;

        size_t len = strlen(file) + strlen(time) + n + strlen(msg) + 64;
        errorMsg = new char[len+1];
        errorMsg[0] = '\0';
        sprintf(errorMsg, "  file: %s\n  line: %-8d\n  time: %s%s  error: %s\n\0",  file, line, time, flag ? "" : "\n",msg);
    }
    catch (bad_alloc &exc) {
        throw;
    }
}
QRException::~QRException() {
    delete[] errorMsg;
}
const char* QRException::what() const noexcept {
    return errorMsg;
}


QRBadAllocException::QRBadAllocException(const char *file, int line, const char *time,
                                               const char *msg, size_t sz)
:QRException(file, line, time, msg), size(sz) {}

const char* QRBadAllocException::what() const noexcept {
    try {
        const char *buf = "QR Bad Alloc Error!\n  Memory amount requested:";
        size_t len = strlen(errorMsg) + strlen(buf) + 64;

        char* msg = new char[len + 1];
        msg[0] = '\0';

        sprintf(msg, "%s %-8zd: %s", buf, size, errorMsg);

        delete[] errorMsg;
        const_cast<QRBadAllocException*>(this)->errorMsg = msg;

        return errorMsg;
    }
    catch (bad_alloc &exc) {
        throw;
    }
}

QRBadPointerException::QRBadPointerException(const char *file, int line, const char *time,
                                         const char *msg)
        :QRException(file, line, time, msg){}

const char* QRBadPointerException::what() const noexcept {
    try {
        const char *buf = "QR Bad Pointer Error!\n  Pointer is invalid";
        size_t len = strlen(errorMsg) + strlen(buf) + 64;

        char* msg = new char[len + 1];
        msg[0] = '\0';

        sprintf(msg, "%s\n %s", buf, errorMsg);

        delete[] errorMsg;
        const_cast<QRBadPointerException*>(this)->errorMsg = msg;

        return errorMsg;
    }
    catch (bad_alloc &exc) {
        throw;
    }
}

QRBadParamException::QRBadParamException(const char *file, int line, const char *time,
                                         const char *msg)
        :QRException(file, line, time, msg){}

const char* QRBadParamException::what() const noexcept {
    try {
        const char *buf = "Bad parameter exception!\n";
        size_t len = strlen(errorMsg) + strlen(buf) + 64;

        char* msg = new char[len + 1];
        msg[0] = '\0';

        sprintf(msg, "%s\n %s", buf, errorMsg);

        delete[] errorMsg;
        const_cast<QRBadParamException*>(this)->errorMsg = msg;

        return errorMsg;
    }
    catch (bad_alloc &exc) {
        throw;
    }
}