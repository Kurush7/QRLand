//
// Created by kurush on 29.08.2020.
//

#include "QuickRenderData.h"

void QuickMatrix::mult(float *v) const {
    tmp[0] = matrix[0][0]*v[0]+matrix[0][1]*v[1] + matrix[0][2]*v[2] + add[0];
    tmp[1] = matrix[1][0]*v[0]+matrix[1][1]*v[1] + matrix[1][2]*v[2] + add[1];
    tmp[2] = matrix[2][0]*v[0]+matrix[2][1]*v[1] + matrix[2][2]*v[2] + add[2];
    v[0] = tmp[0], v[1] = tmp[1], v[2] = tmp[2];
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

std::mutex pointMutex, polyMutex;
int32_t QuickRenderData::addRawPoint(size_t raw_ind) {
    //pointMutex.lock();
    Vector3D v = raw_points[raw_ind]->getVector();
    int32_t x = 3*pointsSize;
    point_arr[x] = v[0];
    point_arr[x+1] = v[1];
    point_arr[x+2] = v[2];
    myPoints[pointsSize] = &point_arr[x];
    pointCodes[raw_ind] = (pointsSize + points_offset) * 100;
    pointsSize++;
    x = pointsSize-1;
    //pointMutex.unlock();
    return x + points_offset;
}

int32_t QuickRenderData::addPoint(float x, float y, float z) {
    //pointMutex.lock();
    int32_t a = 3*pointsSize;
    point_arr[a] = x;
    point_arr[a+1] = y;
    point_arr[a+2] = z;
    myPoints[pointsSize++] = &point_arr[a];
    x = pointsSize-1;
    //pointMutex.unlock();
    return x + points_offset;
}

void QuickRenderData::addPoly(int32_t* arr, int size, size_t from_ind) {
    //polyMutex.lock();
    size_t sz = polySize;
    polygonSize.push_back(size);
    for (int i = 0; i < size; ++i)
        poly_arr[polySize++] = (arr[i]);
    polygons.push_back(&poly_arr[sz]);
    rawPolyMap.push_back(from_ind);

    // todo optimize
    auto poly = raw_polygons[from_ind];
    Vector3D p0(points[arr[0]][0], points[arr[0]][1], points[arr[0]][2]);
    Vector3D p1(points[arr[1]][0], points[arr[1]][1], points[arr[1]][2]);
    Vector3D p2(points[arr[2]][0], points[arr[2]][1], points[arr[2]][2]);
    auto normal = lenNorm((p1-p0) * (p2-p1));
    normal[3] = -scalar(normal, p0);
    if (poly->where(ZeroVector) != sign(scalar(normal, transZero) + normal[3]))
        normal[0] = -normal[0], normal[1] = -normal[1], normal[2] = -normal[2];

    normals.push_back(normal);
    //polyMutex.unlock();
}