//
// Created by kurush on 29.05.2020.
//

#ifndef KG_VECTOR3D_H
#define KG_VECTOR3D_H

#include <ctime>
#include <iostream>
#include <cmath>
#include <initializer_list>

#include "exceptions/MathException.h"
#include "../QRConstants.h"
#include "Functions.h"

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
    explicit Vector3D(const float [4]);
    Vector3D(const std::initializer_list<float> &);

    Vector3D& operator +=(const Vector3D&);
    Vector3D& operator -=(const Vector3D&);
    Vector3D& operator /=(float);
    float& operator[](int);
    const float& operator[](int) const;

    void normSelf();
    void lenNormSelf();

    float arr[4] = {0.f,0.f,0.f,0.f};
};

bool operator <(const Vector3D &a, const Vector3D&b);

const Vector3D XVector(1,0,0,0);
const Vector3D ZeroVector(0,0,0,0);
const Vector3D YVector(0,1,0,0);
const Vector3D ZVector(0,0,1,0);

Vector3D norm(const Vector3D&); // manage 4th axis value, if 0 - does nothing
Vector3D lenNorm(const Vector3D&);
Vector3D len3Norm(const Vector3D&);
float vectorLen(const Vector3D&);
float vectorLen3(const Vector3D&);
bool operator ==(const Vector3D&, const Vector3D&);
bool operator !=(const Vector3D&, const Vector3D&);
Vector3D operator +(const Vector3D&, const Vector3D&);
Vector3D operator -(const Vector3D&, const Vector3D&);
Vector3D operator *(const Vector3D&, const Vector3D&);  // 3dim-vector product, 4th is 0
Vector3D operator *(const Vector3D&, float);
Vector3D operator *(float, const Vector3D&);
Vector3D operator /(const Vector3D&, float);

float scalar(const Vector3D &a, const Vector3D &b);
float scalar3(const Vector3D &a, const Vector3D &b);
float cos(const Vector3D &a, const Vector3D &b);
float cos3(const Vector3D &a, const Vector3D &b);

// for 2dim-vectors. others ignored
bool isRightRotate(const Vector3D &a, const Vector3D &b, const Vector3D &c);


std::ostream& operator<<(std::ostream &os, const Vector3D &v);

#endif //KG_VECTOR3D_H
