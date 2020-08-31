//
// Created by kurush on 29.08.2020.
//

#ifndef BIG3DFLUFFY_QUICKRENDERDATA_H
#define BIG3DFLUFFY_QUICKRENDERDATA_H

#include "containers/QRContainers.h"
#include "math/QRMath.h"
#include "objects/objects.h"

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

    QuickMatrix& operator=(const Matrix3D &m) {
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

    ~QuickMatrix() {
        if (matrix) {
            delete[] matrix[0], delete[] matrix[1], delete[] matrix[2];
            delete[] matrix;
        }
        delete[] tmp;
        delete[] add;
    }

    void mult(float*);
    void projMult(float*);
    void addPerspective(const Matrix3D &m) {
        dot[0] = m[3][0];
        dot[1] = m[3][1];
        dot[2] = m[3][2];
    }

    float **matrix = nullptr;
    float *tmp = new float[3];
    float *add = new float[3], *dot = new float[3];
};


class QuickRenderData {
public:
    QRVector<int32_t> poly_arr;
    int32_t polySize = 0;
    QRVector<int32_t*> polygons;
    QRVector<Vector3D> normals;
    QRVector<char> polygonSize;
    QRVector<int32_t> rawPolyMap;

    QRVector<float> point_arr;  // by 3
    QRVector<float*> points;
    QRVector<int32_t> pointCodes;  // -1 if point not created, code/100 is mapped index
    int32_t pointsSize = 0;


    void init(size_t polyCnt, size_t pointCnt, const Matrix3D &m,
              sptr<QRPoint3D>* raw_pts, sptr<QRPolygon3D>* raw_polys,
              size_t raw_pnt_cnt, size_t raw_poly_cnt) {
        matrix = m;
        transZero = m * ZeroVector;

        raw_points = raw_pts;
        raw_point_cnt = raw_pnt_cnt;
        raw_polygons = raw_polys;
        raw_polygon_cnt = raw_poly_cnt;

        poly_arr.clear();
        polygons.clear();
        polygonSize.clear();
        rawPolyMap.clear();
        normals.clear();
        polySize = 0;

        point_arr.clear();
        points.clear();
        pointCodes.clear();
        pointsSize = 0;

        point_arr.reserve(2*pointCnt);
        poly_arr.reserve(6*pointCnt);   // todo hardcode
        // todo raise alarms if arrays expanded!!!!!

        pointCodes.reserve(pointCnt);
        pointCodes.setSize(pointCnt);
        memset(pointCodes.getPureArray(), 0, sizeof(int32_t)*pointCnt);
    }

    int32_t addRawPoint(size_t raw_ind) {
        Vector3D v = raw_points[raw_ind]->getVector();
        int32_t x = 3*pointsSize;
        point_arr[x] = v[0];
        point_arr[x+1] = v[1];
        point_arr[x+2] = v[2];
        points.push_back(&point_arr[x]);
        pointCodes[raw_ind] = pointsSize * 100;
        return pointsSize++;
    }

    int32_t addPoint(float x, float y, float z) {
        int32_t a = 3*pointsSize;
        point_arr[a] = x;
        point_arr[a+1] = y;
        point_arr[a+2] = z;
        points.push_back(&point_arr[a]);
        return pointsSize++;
    }

    void addPoly(int32_t* arr, int size, size_t from_ind) {
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

    }

    sptr<QRPoint3D>* raw_points = nullptr;
    sptr<QRPolygon3D>* raw_polygons = nullptr;
    size_t raw_point_cnt, raw_polygon_cnt;

    Vector3D transZero;
    QuickMatrix matrix;
};

void QRMultVecMatrix(float*, float**);

#endif //BIG3DFLUFFY_QUICKRENDERDATA_H
