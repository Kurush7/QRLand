//
// Created by kurush on 29.05.2020.
//

#ifndef KG_MATRIX3D_H
#define KG_MATRIX3D_H

#include "Vector3D.h"
#include <fstream>

class Matrix3D {
public:
    Matrix3D();
    Matrix3D(const std::initializer_list<std::initializer_list<float>> &);

    Matrix3D& operator +=(const Matrix3D&);
    Matrix3D& operator *=(const Matrix3D&);     // TODO too slow to use

    Vector3D& operator[](int);
    const Vector3D& operator[](int) const;

    Vector3D matrix[4];
};

bool operator ==(const Matrix3D&, const Matrix3D&);
bool operator !=(const Matrix3D&, const Matrix3D&);
Matrix3D operator +(const Matrix3D&, const Matrix3D&);
Matrix3D operator *(const Matrix3D&, const Matrix3D&);
Vector3D operator *(const Matrix3D&, const Vector3D&);  // todo v[3] considered to be 1!!!

Matrix3D makeID();
Matrix3D makeNull();

Matrix3D transpose(const Matrix3D&);
Matrix3D inverse(const Matrix3D&);  // works only for move & rotate stuff, no projections

std::ostream& operator<<(std::ostream &os, const Matrix3D &m);


#endif //KG_MATRIX3D_H
