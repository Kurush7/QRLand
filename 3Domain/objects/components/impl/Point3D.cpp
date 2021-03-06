//
// Created by kurush on 26.06.2020.
//

#include "Point3D.h"

Point3D::Point3D(const Vector3D &_vec, QRPointStyle s): QRPoint3D(s), vec(_vec) {vec[3] = 0;}

Point3D::Point3D(const float x, const float y, const float z, QRPointStyle s): QRPoint3D(s), vec(x, y, z, 0) {}