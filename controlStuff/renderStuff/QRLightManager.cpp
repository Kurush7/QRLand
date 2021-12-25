//
// Created by kurush on 03.07.2020.
//

#include "QRLightManager.h"

bool QRLightManager::isShaded(const Vector3D &p) {
    Vector3D v = p;
    v = reProjectMatrix * v;
    int x = round(v[0]), y = round(v[1]);
    return shadeZBuf[y*shadeW + x] < v[2] - worldStep;
}