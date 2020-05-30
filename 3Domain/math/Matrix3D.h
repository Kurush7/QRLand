//
// Created by kurush on 29.05.2020.
//

#ifndef KG_MATRIX3D_H
#define KG_MATRIX3D_H

#include "Vector3D.h"

class Matrix3D {
public:
    Matrix3D();
    Matrix3D(const std::initializer_list<std::initializer_list<double>> &);

    Matrix3D& operator +=(const Matrix3D&);
    Matrix3D& operator *=(const Matrix3D&);

    Vector3D& operator[](int);
    const Vector3D& operator[](int) const;

    Vector3D matrix[4];
};

bool operator ==(const Matrix3D&, const Matrix3D&);
bool operator !=(const Matrix3D&, const Matrix3D&);
Matrix3D operator +(const Matrix3D&, const Matrix3D&);
Matrix3D operator *(const Matrix3D&, const Matrix3D&);
Vector3D operator *(const Matrix3D&, const Vector3D&);

Matrix3D makeID();
Matrix3D makeNull();


#endif //KG_MATRIX3D_H