//
// Created by kurush on 29.05.2020.
//

#include "MathException.h"

using namespace std;

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