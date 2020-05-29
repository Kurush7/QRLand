//
// Created by kurush on 29.05.2020.
//

#include "Transformer3D.h"

using namespace std;

MoveTransformer3DCreator::MoveTransformer3DCreator(double dx, double dy, double dz) {
    matrix[3][0] = dx, matrix[3][1] = dy, matrix[3][2] = dz;
}
unique_ptr<BaseTransformer3D> MoveTransformer3DCreator::create() {
    return unique_ptr<BaseTransformer3D>(new Transformer3D(matrix));
}

ScaleTransformer3DCreator::ScaleTransformer3DCreator(double kx, double ky, double kz) {
    matrix[0][0] = kx, matrix[1][1] = ky, matrix[2][2] = kz;
}
unique_ptr<BaseTransformer3D> ScaleTransformer3DCreator::create() {
    return unique_ptr<BaseTransformer3D>(new Transformer3D(matrix));
}


RotateTransformer3DCreator::RotateTransformer3DCreator(double dx, double dy, double dz) {
    if (fabs(dx) > EPS)
        matrix *= createOneRotateMatrix(dx, ox);
    if (fabs(dy) > EPS)
        matrix *= createOneRotateMatrix(dy, oy);
    if (fabs(dz) > EPS)
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

unique_ptr<BaseTransformer3D> RotateTransformer3DCreator::create() {
    return unique_ptr<BaseTransformer3D>(new Transformer3D(matrix));
}

