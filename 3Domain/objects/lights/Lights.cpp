//
// Created by kurush on 30.06.2020.
//

#include "Lights.h"

Vector3D BaseLight::getIntensity(const Vector3D &normal) {
    Vector3D i(0,0,0,0);
    i += ambient;
    return i;
    // todo!!!
}