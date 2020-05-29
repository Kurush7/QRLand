//
// Created by kurush on 28.04.2020.
//

#ifndef OOP_SETERROR_H
#define OOP_SETERROR_H

#include <exception>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "../QRExceptions/QRContainerException.cpp"

class SetException: public QRContainerException {
public:
    explicit SetException(const char *file, int line, const char *time, const char *msg);
    virtual ~SetException();
    virtual const char* what() const noexcept override;

protected:
    char* errorMsg;
};


class SetExceptionBadPointer : public SetException
{
public:
    SetExceptionBadPointer(const char *file, int line, const char *time, const char *msg);
    virtual ~SetExceptionBadPointer() = default;

    virtual const char* what() const noexcept override;
};

#endif //OOP_SETERROR_H
