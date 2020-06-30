//
// Created by kurush on 30.06.2020.
//

#ifndef BIG3DFLUFFY_QRLIGHTINTERFACE_H
#define BIG3DFLUFFY_QRLIGHTINTERFACE_H

#include "math/QRMath.h"

class QRLight {
public:
    virtual Vector3D getPosition() = 0;
    virtual Vector3D getIntensity(const Vector3D &normal) = 0; //todo
};


#endif //BIG3DFLUFFY_QRLIGHTINTERFACE_H
