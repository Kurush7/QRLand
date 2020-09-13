//
// Created by kurush on 10.09.2020.
//

#ifndef BIG3DFLUFFY_QUICKMATH_H
#define BIG3DFLUFFY_QUICKMATH_H

#include "Matrix3D.h"

struct QuickMatrix {
    QuickMatrix() {
        matrix = new float*[3];
        matrix[0] = new float[3];
        matrix[1] = new float[3];
        matrix[2] = new float[3];
    }
    QuickMatrix(const Matrix3D &m): QuickMatrix() {
        operator=(m);
    }
    QuickMatrix& operator=(const Matrix3D &m);
    ~QuickMatrix() {
        if (matrix) {
            delete[] matrix[0], delete[] matrix[1], delete[] matrix[2];
            delete[] matrix;
        }
        delete[] tmp;
        delete[] add;
    }

    void mult(float*) const;
    void bareMult(float*) const; // no move or perspective
    Vector3D bareMult(const Vector3D&) const; // no move or perspective
    void projMult(float*) const;    // 2d-product
    void addPerspective(const Matrix3D &m) {
        dot[0] = m[3][0], dot[1] = m[3][1], dot[2] = m[3][2];
    }

    float **matrix = nullptr;
    float *tmp = new float[3];
    float *add = new float[3], *dot = new float[3];
};

#endif //BIG3DFLUFFY_QUICKMATH_H
