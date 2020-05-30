//
// Created by kurush on 29.05.2020.
//

#ifndef KG_MATHEXCEPTION_H
#define KG_MATHEXCEPTION_H

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


#endif //KG_MATHEXCEPTION_H
