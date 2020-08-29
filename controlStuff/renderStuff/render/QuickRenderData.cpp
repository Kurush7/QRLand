//
// Created by kurush on 29.08.2020.
//

#include "QuickRenderData.h"

void QuickMatrix::mult(float *v) {
    float x = dot[0]*v[0] + dot[1]*v[1] + dot[2]*v[2] + 1;
    tmp[0] = matrix[0][0]*v[0]+matrix[0][1]*v[1] + matrix[0][2]*v[2] + add[0];
    tmp[1] = matrix[1][0]*v[0]+matrix[1][1]*v[1] + matrix[1][2]*v[2] + add[1];
    tmp[2] = matrix[2][0]*v[0]+matrix[2][1]*v[1] + matrix[2][2]*v[2] + add[2];
    v[0] = tmp[0] / x, v[1] = tmp[1] / x, v[2] = tmp[2] / x;
}