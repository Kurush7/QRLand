//
// Created by kurush on 27.06.2020.
//

#ifndef BIG3DFLUFFY_FUNCTIONS_H
#define BIG3DFLUFFY_FUNCTIONS_H

#include "QRConstants.h"

// todo wtf - not linking when impl is in cpp
inline char sign(float x) {
    return x > QREPS? 1 : (x < -QREPS? -1 : 0);
}

// todo not ints
inline int QRound(float x) {
    int y = x;
    return x >= y + 0.5? y+1 : y;
}

#endif //BIG3DFLUFFY_FUNCTIONS_H
