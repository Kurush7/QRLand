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

mutex pointCodesMutex;
int32_t QuickRenderData::addRawPoint(size_t raw_ind) {
    Vector3D v = raw_points[raw_ind]->getVector();
    int32_t x = 3*pointsSize;
    point_arr[x] = v[0];
    point_arr[x+1] = v[1];
    point_arr[x+2] = v[2];
    myPoints[pointsSize] = &point_arr[x];
    // pointCodes should update here by (points_offset + pointsSize)*100
    // but to avoid many mutexes it's done in Quick3DCutter after code computing
    return points_offset + pointsSize++;
}

int32_t QuickRenderData::addPoint(float x, float y, float z) {
    int32_t a = 3*pointsSize;
    point_arr[a] = x;
    point_arr[a+1] = y;
    point_arr[a+2] = z;
    myPoints[pointsSize] = &point_arr[a];
    return points_offset + pointsSize++;
}

void QuickRenderData::addPoly(int32_t* arr, int size, size_t from_ind) {
    polygonSize.push_back(size);
    for (int i = 0; i < size; ++i)
        poly_arr[polySize++] = (arr[i]);
    polygons.push_back(&poly_arr[polySize-size]);
    rawPolyMap.push_back(from_ind);

    //normals.push_back(matrix.bareMult(raw_polygons[from_ind]->getNormal()));
    normals.push_back(modelMatrix.bareMult(raw_polygons[from_ind]->getNormal()));   // todo only move in modelmatrix - useless
    //normals.push_back(raw_polygons[from_ind]->getNormal());
}

void QuickRenderMetaData::init(const Matrix3D &m, const Matrix3D &modelMatrix, sptr<QRPoint3D>* raw_pts, sptr<QRPolygon3D>* raw_polys,
          size_t raw_pnt_cnt, size_t raw_poly_cnt) {
    matrix = m;

    int secure_coef = 8;         // todo control!!! arrays must not expand!!!!!!!!... smth wrong on a cube with no preset sizes
    if (poly_arr_size < raw_poly_cnt*secure_coef)
        poly_arr = new int32_t[raw_poly_cnt*secure_coef], poly_arr_size = raw_poly_cnt*secure_coef;
    if (point_arr_size < raw_pnt_cnt*secure_coef)
        point_arr = new float[raw_pnt_cnt*secure_coef], point_arr_size = raw_pnt_cnt*secure_coef;
    if (points_size < point_arr_size/3)
        points = new float*[raw_pnt_cnt*secure_coef], points_size = point_arr_size/3;

    pointCodes.reserve(raw_pnt_cnt);
    pointCodes.setSize(raw_pnt_cnt);
    memset(pointCodes.getPureArray(), 0, sizeof(int32_t)*raw_pnt_cnt);

    size_t pointStep = point_arr_size / thread_cnt;
    size_t polyStep = poly_arr_size / thread_cnt;
    size_t ptStep = points_size / thread_cnt;
    for (int i = 0; i < thread_cnt; ++i)
        data[i]->init(m, modelMatrix, raw_pts, raw_polys, raw_pnt_cnt, raw_poly_cnt, &poly_arr[i*polyStep],
                      &point_arr[i*pointStep], points, &points[i*ptStep], i*ptStep);
}

void QuickRenderData::init(const Matrix3D &m, const Matrix3D &mM, sptr<QRPoint3D>* raw_pts, sptr<QRPolygon3D>* raw_polys,
          size_t raw_pnt_cnt, size_t raw_poly_cnt,
          int32_t *polyArr, float*pointArr, float** pts, float** myPts, int32_t offset) {
    matrix = m;
    modelMatrix = mM;

    poly_arr = polyArr;
    point_arr = pointArr;
    points = pts;
    myPoints = myPts;
    points_offset = offset;

    raw_points = raw_pts;
    raw_polygons = raw_polys;

    polygons.clear();
    polygonSize.clear();
    rawPolyMap.clear();
    normals.clear();
    polySize = 0;
    pointsSize = 0;
}