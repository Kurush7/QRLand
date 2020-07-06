//
// Created by kurush on 29.05.2020.
//

#ifndef QR_EXCEPTION_H
#define QR_EXCEPTION_H

#include <exception>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "QRDefines.h"

class QRException: public std::exception {
public:
    QRException(const char *file, int line, const char *time, const char *msg);
    virtual ~QRException();
    virtual const char* what() const noexcept override;

protected:
    char* errorMsg;
};

class QRContainerException: public QRException {
public:
    QRContainerException(const char *file, int line, const char *time, const char *msg):
            QRException(file, line, time, msg) {}
};

class QRObjectException: public QRException {
public:
    QRObjectException(const char *file, int line, const char *time, const char *msg):
            QRException(file, line, time, msg) {}
};


class QRBadAllocException: public QRException
{
public:
    QRBadAllocException(const char *file, int line, const char *time, const char *msg, size_t sz);
    virtual ~QRBadAllocException() = default;

    virtual const char* what() const noexcept override;

private:
    size_t size;
};


class QRBadPointerException: public QRException
{
public:
    QRBadPointerException(const char *file, int line, const char *time, const char *msg);
    virtual ~QRBadPointerException() = default;

    virtual const char* what() const noexcept override;
};


#endif //QR_EXCEPTION_H
