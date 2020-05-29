//
// Created by kurush on 28.04.2020.
//

#include "HashTableError.h"

using namespace std;

HashTableException::HashTableException(const char *file, int line, const char *time, const char *msg) {
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

HashTableException::~HashTableException() {
    delete[] errorMsg;
}

const char* HashTableException::what() const noexcept {
    return errorMsg;
}


HashTableBadAlloc::HashTableBadAlloc(const char *file, int line, const char *time,
                                     const char *msg, size_t size)
: HashTableException(file, line, time, msg), size(size) {}

const char* HashTableBadAlloc::what() const noexcept {
    try {
        const char *buf = "<< HashTable Bad Alloc Exception!\n  memory amount requested: ";
        size_t len = strlen(errorMsg) + strlen(buf) + 16;

        char *msg = new char[len + 1];

        sprintf(msg, "%s%-8zd\n%s>>", buf, size, errorMsg);

        delete[] errorMsg;
        const_cast<HashTableBadAlloc *>(this)->errorMsg = msg;

        return errorMsg;
    }
    catch (bad_alloc &exc) {
        throw;
    }
}


HashTableBadIterator::HashTableBadIterator(const char *file, int line, const char *time,
                                           const char *msg)
        : HashTableException(file, line, time, msg) {}

const char* HashTableBadIterator::what() const noexcept {
    try {
        const char *buf = "HashTable Bad Iterator Error!";
        size_t len = strlen(errorMsg) + strlen(buf) + 16;

        char* msg = new char[len + 1];

        sprintf(msg, "%s: %s", buf, errorMsg);

        delete[] errorMsg;
        const_cast<HashTableBadIterator*>(this)->errorMsg = msg;

        return errorMsg;
    }
    catch (bad_alloc &exc) {
        throw;
    }
}
