//
// Created by kurush on 28.04.2020.
//

#ifndef OOP_HASHTABLEERROR_H
#define OOP_HASHTABLEERROR_H

#include <exception>
#include <cstring>
#include <cstdio>
#include "../QRExceptions/QRContainerException.cpp"

class HashTableException: public QRContainerException {
public:
    explicit HashTableException(const char *file, int line, const char *time, const char *msg);
    virtual ~HashTableException();
    virtual const char* what() const noexcept override;

protected:
    char* errorMsg;
};


class HashTableBadIterator: public HashTableException
{
public:
    explicit HashTableBadIterator(const char *file, int line, const char *time, const char *msg);
    virtual ~HashTableBadIterator() = default;

    virtual const char* what() const noexcept override;
};

#endif //OOP_HASHTABLEERROR_H
