//
// Created by kurush on 28.04.2020.
//

#ifndef OOP_HASHTABLEERROR_H
#define OOP_HASHTABLEERROR_H

#include "QRContainerException.h"

class QRHashTableException: public QRContainerException {
public:
    explicit QRHashTableException(const char *file, int line, const char *time, const char *msg);
    virtual ~QRHashTableException();
    virtual const char* what() const noexcept override;

protected:
    char* errorMsg;
};


class QRHashTableBadIterator: public QRHashTableException
{
public:
    explicit QRHashTableBadIterator(const char *file, int line, const char *time, const char *msg);
    virtual ~QRHashTableBadIterator() = default;

    virtual const char* what() const noexcept override;
};

#endif //OOP_HASHTABLEERROR_H
