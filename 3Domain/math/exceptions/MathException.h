//
// Created by kurush on 29.05.2020.
//

#ifndef KG_QRCONTAINEREXCEPTION_H
#define KG_QRCONTAINEREXCEPTION_H

#include <exception>
#include <cstring>
#include <cstdio>
#include <cstdlib>

class QRMathException: public std::exception {
public:
    explicit QRMathException(const char *file, int line, const char *time, const char *msg);
    virtual ~QRMathException();
    virtual const char* what() const noexcept override;

protected:
    char* errorMsg;
};

class QRMathWrongDimension: public QRMathException
{
public:
    QRMathWrongDimension(const char *file, int line, const char *time, const char *msg, int sent, int expected);
    virtual ~QRMathWrongDimension() = default;

    virtual const char* what() const noexcept override;

private:
    int dim1, dim2;
};



#endif //KG_QRCONTAINEREXCEPTION_H
