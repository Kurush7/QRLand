//
// Created by kurush on 29.05.2020.
//

#include "MathException.h"

using namespace std;

QRMathException::QRMathException(const char *file, int line, const char *time, const char *msg) {
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
QRMathException::~QRMathException() {
    delete[] errorMsg;
}
const char* QRMathException::what() const noexcept {
    return errorMsg;
}


QRMathWrongDimension::QRMathWrongDimension(const char *file, int line, const char *time,
                                           const char *msg, int sent, int expected): QRMathException(file, line, time, msg),
                                                                          dim1(sent), dim2(expected) {}
const char* QRMathWrongDimension::what() const noexcept {
    try {
        const char *buf = "<< Math Bad Dimension Exception!\n  dimension requested: ";
        const char *buf2 = "\n  dimension expected: ";
        size_t len = strlen(errorMsg) + strlen(buf) + strlen(buf2) + 64;

        char *msg = new char[len + 1];

        sprintf(msg, "%s%-8zd%s%-8zd%s>>", buf, dim1, buf2, dim2,  errorMsg);

        delete[] errorMsg;
        const_cast<QRMathWrongDimension *>(this)->errorMsg = msg;

        return errorMsg;
    }
    catch (bad_alloc &exc) {
        throw;
    }
}