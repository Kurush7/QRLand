//
// Created by kurush on 29.05.2020.
//

#ifndef KG_VECTOR3D_H
#define KG_VECTOR3D_H

#include <ctime>
#include <cmath>

#include <initializer_list>

#include "exceptions/MathException.h"

// todo problem: consider arr[3] or not when operating?

const double QREPS = 1e-9;

class Vector3D {
public:
    Vector3D();
    Vector3D(double, double, double);
    explicit Vector3D(const double [4]);
    Vector3D(const std::initializer_list<double> &);

    Vector3D& operator +=(const Vector3D&);
    Vector3D& operator -=(const Vector3D&);
    Vector3D& operator /=(double);
    double& operator[](int);
    const double& operator[](int) const;

    double arr[4] = {0,0,0,1};
};

Vector3D norm(const Vector3D&);
bool operator ==(const Vector3D&, const Vector3D&);
bool operator !=(const Vector3D&, const Vector3D&);
Vector3D operator +(const Vector3D&, const Vector3D&);
Vector3D operator -(const Vector3D&, const Vector3D&);
Vector3D operator *(const Vector3D&, const Vector3D&);
Vector3D operator /(const Vector3D&, double);

#endif //KG_VECTOR3D_H
