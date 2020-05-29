//
// Created by kurush on 29.05.2020.
//

#ifndef KG_QRCONTAINEREXCEPTION_H
#define KG_QRCONTAINEREXCEPTION_H

#include <exception>
#include <cstring>
#include <cstdio>
#include <cstdlib>

class QRContainerException: public std::exception {
public:
    explicit QRContainerException(const char *file, int line, const char *time, const char *msg);
    virtual ~QRContainerException();
    virtual const char* what() const noexcept override;

protected:
    char* errorMsg;
};

class QRBadAllocException: public QRContainerException
{
public:
    QRBadAllocException(const char *file, int line, const char *time, const char *msg, size_t);
    virtual ~QRBadAllocException() = default;

    virtual const char* what() const noexcept override;

private:
    size_t size;
};



#endif //KG_QRCONTAINEREXCEPTION_H
