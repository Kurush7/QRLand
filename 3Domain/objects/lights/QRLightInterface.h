//
// Created by kurush on 30.06.2020.
//

#ifndef BIG3DFLUFFY_QRLIGHTINTERFACE_H
#define BIG3DFLUFFY_QRLIGHTINTERFACE_H

#include "math/QRMath.h"

class QRLight {
public:
    virtual void transform(const QRTransformer3D *trans) = 0;
    virtual void resetPosition() = 0;
    virtual Vector3D getPosition() = 0;
    virtual Vector3D getLightVector() = 0;
    virtual Vector3D getIntensity(const Vector3D &pos, const Vector3D &normal) = 0;
};


#endif //BIG3DFLUFFY_QRLIGHTINTERFACE_H
