//
// Created by kurush on 30.06.2020.
//

#include "Lights.h"

Vector3D BaseLight::getIntensity(const Vector3D &pos, const Vector3D &normal) {
    Vector3D i(0,0,0,0);
    i += ambient;
    i += diffuse * cos(-1*normal, lightDir);    // -1: angle between returning light, not falling...
    //std::cout << "***" << normal << ' ' << lightDir << ' ' << cos(normal, lightDir) << '\n';
    return i;
    // todo!!!
}