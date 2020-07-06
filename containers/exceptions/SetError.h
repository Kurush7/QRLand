//
// Created by kurush on 28.04.2020.
//

#ifndef OOP_SETERROR_H
#define OOP_SETERROR_H

#include "QRContainerException.h"

class QRSetException: public QRContainerException {
public:
    explicit QRSetException(const char *file, int line, const char *time, const char *msg);
    virtual ~QRSetException();
    virtual const char* what() const noexcept override;

protected:
    char* errorMsg;
};


class QRSetExceptionBadPointer : public QRSetException
{
public:
    QRSetExceptionBadPointer(const char *file, int line, const char *time, const char *msg);
    virtual ~QRSetExceptionBadPointer() = default;

    virtual const char* what() const noexcept override;
};

#endif //OOP_SETERROR_H
