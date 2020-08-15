//
// Created by kurush on 28.04.2020.
//

#include "HashTableError.h"

using namespace std;

QRHashTableException::QRHashTableException(const char *file, int line, const char *time, const char *msg)
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

QRHashTableException::~QRHashTableException() {
    delete[] errorMsg;
}

const char* QRHashTableException::what() const noexcept {
    return errorMsg;
}


QRHashTableBadIterator::QRHashTableBadIterator(const char *file, int line, const char *time,
                                               const char *msg)
        : QRHashTableException(file, line, time, msg) {}

const char* QRHashTableBadIterator::what() const noexcept {
    try {
        const char *buf = "HashTable Bad Iterator Error!";
        size_t len = strlen(errorMsg) + strlen(buf) + 16;

        char* msg = new char[len + 1];

        sprintf(msg, "%s: %s", buf, errorMsg);

        delete[] errorMsg;
        const_cast<QRHashTableBadIterator*>(this)->errorMsg = msg;

        return errorMsg;
    }
    catch (bad_alloc &exc) {
        throw;
    }
}
