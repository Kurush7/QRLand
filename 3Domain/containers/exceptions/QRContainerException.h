//
// Created by kurush on 29.05.2020.
//

#ifndef KG_MATHEXCEPTION_H
#define KG_MATHEXCEPTION_H

#include "../../exceptions/QRException.h"

class QRBadPointerException: public QRContainerException
{
public:
    QRBadPointerException(const char *file, int line, const char *time, const char *msg);
    virtual ~QRBadPointerException() = default;

    virtual const char* what() const noexcept override;
};



#endif //KG_MATHEXCEPTION_H
