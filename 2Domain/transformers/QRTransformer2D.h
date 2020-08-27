//
// Created by kurush on 19.08.2020.
//

#ifndef BIG3DFLUFFY_QRTRANSFORMER2D_H
#define BIG3DFLUFFY_QRTRANSFORMER2D_H

#include "math/QRMath.h"

class QRTransformer2D {
public:
    virtual Vector3D transform(const Vector3D &vec) {return vec;}
};


#endif //BIG3DFLUFFY_QRTRANSFORMER2D_H
