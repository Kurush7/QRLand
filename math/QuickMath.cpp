//
// Created by kurush on 10.09.2020.
//

#include "QuickMath.h"

void QuickMatrix::mult(float *v) const {
    tmp[0] = matrix[0][0]*v[0]+matrix[0][1]*v[1] + matrix[0][2]*v[2] + add[0];
    tmp[1] = matrix[1][0]*v[0]+matrix[1][1]*v[1] + matrix[1][2]*v[2] + add[1];
    tmp[2] = matrix[2][0]*v[0]+matrix[2][1]*v[1] + matrix[2][2]*v[2] + add[2];
    v[0] = tmp[0], v[1] = tmp[1], v[2] = tmp[2];
}

void QuickMatrix::bareMult(float *v) const {
    tmp[0] = matrix[0][0]*v[0]+matrix[0][1]*v[1] + matrix[0][2]*v[2];
    tmp[1] = matrix[1][0]*v[0]+matrix[1][1]*v[1] + matrix[1][2]*v[2];
    tmp[2] = matrix[2][0]*v[0]+matrix[2][1]*v[1] + matrix[2][2]*v[2];
    v[0] = tmp[0], v[1] = tmp[1], v[2] = tmp[2];
}

Vector3D QuickMatrix::bareMult(const Vector3D &v) const {
    Vector3D tmp;
    tmp[0] = matrix[0][0]*v[0]+matrix[0][1]*v[1] + matrix[0][2]*v[2];
    tmp[1] = matrix[1][0]*v[0]+matrix[1][1]*v[1] + matrix[1][2]*v[2];
    tmp[2] = matrix[2][0]*v[0]+matrix[2][1]*v[1] + matrix[2][2]*v[2];
    return tmp;
}

void QuickMatrix::projMult(float *v) const {
    float x = dot[0]*v[0] + dot[1]*v[1] + dot[2]*v[2];
    tmp[0] = matrix[0][0]*v[0]+matrix[0][1]*v[1] + matrix[0][2]*v[2];
    tmp[1] = matrix[1][0]*v[0]+matrix[1][1]*v[1] + matrix[1][2]*v[2];
    v[0] = tmp[0] / x + add[0], v[1] = tmp[1] / x + add[1];
}

QuickMatrix& QuickMatrix::operator=(const Matrix3D &m) {
    // scale-rotates
    matrix[0][0] = m[0][0];
    matrix[0][1] = m[0][1];
    matrix[0][2] = m[0][2];
    matrix[1][0] = m[1][0];
    matrix[1][1] = m[1][1];
    matrix[1][2] = m[1][2];
    matrix[2][0] = m[2][0];
    matrix[2][1] = m[2][1];
    matrix[2][2] = m[2][2];
    // moves
    add[0] = m[0][3];
    add[1] = m[1][3];
    add[2] = m[2][3];
    // perspectives
    dot[0] = m[3][0];
    dot[1] = m[3][1];
    dot[2] = m[3][2];
}