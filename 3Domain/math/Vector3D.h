//
// Created by kurush on 29.05.2020.
//

#ifndef KG_VECTOR3D_H
#define KG_VECTOR3D_H

#include <ctime>
#include <cmath>
#include <initializer_list>

#include "exceptions/MathException.h"
#include "QRConstants.h"

/**
4-dimensional vector with no regard to projective coordinates (no special management for 4-th value),
so projective functions using this class are bound to manage it themselves
todo: make special structure
 */

class Vector3D {
public:
    Vector3D();
    Vector3D(double, double, double);
    Vector3D(double, double, double, double);
    explicit Vector3D(const double [4]);
    Vector3D(const std::initializer_list<double> &);

    Vector3D& operator +=(const Vector3D&);
    Vector3D& operator -=(const Vector3D&);
    Vector3D& operator /=(double);
    double& operator[](int);
    const double& operator[](int) const;

    double arr[4] = {0,0,0,0};
};

Vector3D norm(const Vector3D&); // manage 4th axis value
Vector3D lenNorm(const Vector3D&);
bool operator ==(const Vector3D&, const Vector3D&);
bool operator !=(const Vector3D&, const Vector3D&);
Vector3D operator +(const Vector3D&, const Vector3D&);
Vector3D operator -(const Vector3D&, const Vector3D&);
Vector3D operator *(const Vector3D&, const Vector3D&);  // vector product
Vector3D operator *(const Vector3D&, double);
Vector3D operator *(double, const Vector3D&);
Vector3D operator /(const Vector3D&, double);

#endif //KG_VECTOR3D_H
