//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_QRQUEUE_H
#define BIG3DFLUFFY_QRQUEUE_H

#include "QRVector.h"

template <typename T>
class QRQueue {
public:
    QRQueue() {}
    void push(const T &x) {vector.push_back(x);}
    T pop() {
        if (vector.isEmpty())
            throw ErrorIndex(__FILE__, __LINE__, __TIME__, "pop from empty queue");
        T res = vector[0];
        for (size_t i = 0; i < vector.len() - 1; ++i)
            vector[i] = vector[i+1];
        vector.pop_back();
        return res;
    }
private:
    QRVector<T> vector;
};


#endif //BIG3DFLUFFY_QRQUEUE_H
