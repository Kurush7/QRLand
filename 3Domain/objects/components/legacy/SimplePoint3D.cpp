//
// Created by kurush on 24.06.2020.
//

#include "SimplePoint3D.h"

SimplePoint3D::SimplePoint3D(const Vector3D &_vec, const Vector3D &bind, QRPointStyle s): QRPoint3D(s),
                                                                                          vec(_vec), bindPoint(bind) {
    vec -= bind;
}

SimplePoint3D::SimplePoint3D(const float x, const float y, const float z, const Vector3D &bind, QRPointStyle s)
        : QRPoint3D(s), vec(x, y, z), bindPoint(bind) {
    vec -= bind;
}