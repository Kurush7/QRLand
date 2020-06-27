//
// Created by kurush on 29.05.2020.
//

#ifndef QR_OBJECTEXCEPTION_H
#define QR_OBJECTEXCEPTION_H

#include <exception>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "../../exceptions/QRException.h"


class QRBadPointException: public QRObjectException
{
public:
    QRBadPointException(const char *file, int line, const char *time, const char *msg);
    virtual ~QRBadPointException() = default;

    virtual const char* what() const noexcept override;
};

class QRBadSourceException: public QRObjectException
{
public:
    QRBadSourceException(const char *file, int line, const char *time, const char *msg);
    virtual ~QRBadSourceException() = default;

    virtual const char* what() const noexcept override;
};

class QRBadSizeException: public QRObjectException
{
public:
    QRBadSizeException(const char *file, int line, const char *time, const char *msg);
    virtual ~QRBadSizeException() = default;

    virtual const char* what() const noexcept override;
};




#endif //QR_OBJECTEXCEPTION_H
