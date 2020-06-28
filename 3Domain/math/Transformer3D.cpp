//
// Created by kurush on 29.05.2020.
//

#include "Transformer3D.h"

using namespace std;

MoveTransformer3DCreator::MoveTransformer3DCreator(double dx, double dy, double dz) {
    matrix = makeID();
    matrix[0][3] = dx, matrix[1][3] = dy, matrix[2][3] = dz;
}
uptr<QRTransformer3D> MoveTransformer3DCreator::create() {
    return uptr<QRTransformer3D>(new Transformer3D(matrix));
}

ScaleTransformer3DCreator::ScaleTransformer3DCreator(double kx, double ky, double kz) {
    matrix = makeID();
    matrix[0][0] = kx, matrix[1][1] = ky, matrix[2][2] = kz;
}
uptr<QRTransformer3D> ScaleTransformer3DCreator::create() {
    return uptr<QRTransformer3D>(new Transformer3D(matrix));
}


RotateTransformer3DCreator::RotateTransformer3DCreator(double dx, double dy, double dz) {
    matrix = makeID();
    if (fabs(dx) > QREPS)
        matrix *= createOneRotateMatrix(dx, ox);
    if (fabs(dy) > QREPS)
        matrix *= createOneRotateMatrix(dy, oy);
    if (fabs(dz) > QREPS)
        matrix *= createOneRotateMatrix(dz, oz);
}

Matrix3D RotateTransformer3DCreator::createOneRotateMatrix(double rad, axis ax) {
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


ProjectionTransformer3DCreator::ProjectionTransformer3DCreator(const Vector3D &base,
        const Vector3D &ox, const Vector3D &oy, const Vector3D &oz) {
    MoveTransformer3DCreator mc(-base[0], -base[1], -base[2]);
    matrix = mc.create()->transform(matrix);

    Matrix3D rot;   // combine axis to new ones
    for (int i = 0; i < 3; ++i) {
        rot[0][i] = ox[i];
        rot[1][i] = oy[i];
        rot[2][i] = oz[i];
    }
    matrix = rot * matrix;
}

uptr<QRTransformer3D> ProjectionTransformer3DCreator::create() {
    return uptr<QRTransformer3D>(new Transformer3D(matrix));
}