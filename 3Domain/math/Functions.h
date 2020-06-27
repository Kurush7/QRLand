//
// Created by kurush on 27.06.2020.
//

#ifndef BIG3DFLUFFY_FUNCTIONS_H
#define BIG3DFLUFFY_FUNCTIONS_H

#include "QRConstants.h"

// todo wtf - not linking when impl is in cpp
inline char sign(double x) {
    return x > QREPS? 1 : (x < -QREPS? -1 : 0);
}

#endif //BIG3DFLUFFY_FUNCTIONS_H
