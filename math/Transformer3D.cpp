//
// Created by kurush on 29.05.2020.
//

#include "Transformer3D.h"

using namespace std;

MoveTransformer3DCreator::MoveTransformer3DCreator(float dx, float dy, float dz) {
    matrix = makeID();
    matrix[0][3] = dx, matrix[1][3] = dy, matrix[2][3] = dz;
}
uptr<QRTransformer3D> MoveTransformer3DCreator::create() {
    return uptr<QRTransformer3D>(new Transformer3D(matrix));
}

ScaleTransformer3DCreator::ScaleTransformer3DCreator(float kx, float ky, float kz) {
    matrix = makeID();
    matrix[0][0] = kx, matrix[1][1] = ky, matrix[2][2] = kz;
}
uptr<QRTransformer3D> ScaleTransformer3DCreator::create() {
    return uptr<QRTransformer3D>(new Transformer3D(matrix));
}


RotateTransformer3DCreator::RotateTransformer3DCreator(float dx, float dy, float dz) {
    matrix = makeID();
    if (fabs(dx) > QREPS)
        matrix *= createOneRotateMatrix(dx, ox);
    if (fabs(dy) > QREPS)
        matrix *= createOneRotateMatrix(dy, oy);
    if (fabs(dz) > QREPS)
        matrix *= createOneRotateMatrix(dz, oz);
}

Matrix3D RotateTransformer3DCreator::createOneRotateMatrix(float rad, axis ax) {
    Matrix3D matr = makeID();
    if (ax == ox) {
        matr[1][1] = cos(rad);
        matr[1][2] = -sin(rad);
        matr[2][1] = sin(rad);
        matr[2][2] = cos(rad);
    } else if (ax == oy) {
        matr[0][0] = cos(rad);
        matr[0][2] = -sin(rad);
        matr[2][0] = sin(rad);
        matr[2][2] = cos(rad);
    } else {
        matr[0][0] = cos(rad);
        matr[0][1] = -sin(rad);
        matr[1][0] = sin(rad);
        matr[1][1] = cos(rad);
    }
    return matr;
}

uptr<QRTransformer3D> RotateTransformer3DCreator::create() {
    return uptr<QRTransformer3D>(new Transformer3D(matrix));
}


ProjectionTransformer3DCreator::ProjectionTransformer3DCreator(const Vector3D &v) {
    if (fabs(v[0]) > QREPS) matrix[3][0] = 1/v[0] * PROJECTIVE_COEF;
    if (fabs(v[1]) > QREPS) matrix[3][1] = 1/v[1] * PROJECTIVE_COEF;
    if (fabs(v[2]) > QREPS) matrix[3][2] = 1/v[2] * PROJECTIVE_COEF;
}
ProjectionTransformer3DCreator::ProjectionTransformer3DCreator(float x, float y, float z)
:ProjectionTransformer3DCreator(Vector3D(x,y,z)) {}


uptr<QRTransformer3D> ProjectionTransformer3DCreator::create() {
    return uptr<QRTransformer3D>(new Transformer3D(matrix));
}

AxisChangeTransformer::AxisChangeTransformer(const Vector3D &oX, const Vector3D &oY,
        const Vector3D &oZ, const Vector3D &origin) {
    for (int i = 0; i < 3; ++i) {
        matrix[0][i] = oX[i];
        matrix[1][i] = oY[i];
        matrix[2][i] = oZ[i];
    }
    matrix[0][3] = -origin[0];
    matrix[1][3] = -origin[1];
    matrix[2][3] = -origin[2];
}