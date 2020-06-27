//
// Created by kurush on 29.05.2020.
//

#include "QRObjectException.h"

using namespace std;

QRBadPointException::QRBadPointException(const char *file, int line, const char *time,
                                             const char *msg)
        :QRObjectException(file, line, time, msg){}

const char* QRBadPointException::what() const noexcept {
    try {
        const char *buf = "Point link is invalid!\n";
        size_t len = strlen(errorMsg) + strlen(buf) + 64;

        char* msg = new char[len + 1];
        msg[0] = '\0';

        sprintf(msg, "%s\n %s", buf, errorMsg);

        delete[] errorMsg;
        const_cast<QRBadPointException*>(this)->errorMsg = msg;

        return errorMsg;
    }
    catch (bad_alloc &exc) {
        throw;
    }
}

QRBadSourceException::QRBadSourceException(const char *file, int line, const char *time,
                                         const char *msg)
        :QRObjectException(file, line, time, msg){}

const char* QRBadSourceException::what() const noexcept {
    try {
        const char *buf = "Data source is not found or invalid!\n";
        size_t len = strlen(errorMsg) + strlen(buf) + 64;

        char* msg = new char[len + 1];
        msg[0] = '\0';

        sprintf(msg, "%s\n %s", buf, errorMsg);

        delete[] errorMsg;
        const_cast<QRBadSourceException*>(this)->errorMsg = msg;

        return errorMsg;
    }
    catch (bad_alloc &exc) {
        throw;
    }
}


QRBadSizeException::QRBadSizeException(const char *file, int line, const char *time,
                                           const char *msg)
        :QRObjectException(file, line, time, msg){}

const char* QRBadSizeException::what() const noexcept {
    try {
        const char *buf = "Bad size exception! (probably too little points for a polygon)\n";
        size_t len = strlen(errorMsg) + strlen(buf) + 64;

        char* msg = new char[len + 1];
        msg[0] = '\0';

        sprintf(msg, "%s\n %s", buf, errorMsg);

        delete[] errorMsg;
        const_cast<QRBadSizeException*>(this)->errorMsg = msg;

        return errorMsg;
    }
    catch (bad_alloc &exc) {
        throw;
    }
}