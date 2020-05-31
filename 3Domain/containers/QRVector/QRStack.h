//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_QRSTACK_H
#define BIG3DFLUFFY_QRSTACK_H


#include "QRVector.h"

template <typename T>
class QRStack {
public:
    QRStack() {}
    void push(const T &x) {vector.push_back(x);}
    T pop() {
        return vector.pop_back();
    }
private:
    QRVector<T> vector;
};

#endif //BIG3DFLUFFY_QRSTACK_H
