//
// Created by kurush on 29.08.2020.
//

#ifndef BIG3DFLUFFY_QUICKRENDERDATA_H
#define BIG3DFLUFFY_QUICKRENDERDATA_H

#include <thread>
#include <mutex>

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
    QuickMatrix& operator=(const Matrix3D &m);
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
        dot[0] = m[3][0], dot[1] = m[3][1], dot[2] = m[3][2];
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

    QRVector<float> point_arr;  // by triples of {x,y,z}
    QRVector<float*> points;
    QRVector<int32_t> pointCodes;  // 0 if point not created, code/100 is mapped index  // todo bug for 0'th point to be parsed twice if it's visible
    int32_t pointsSize = 0;

    void init(const Matrix3D &m, sptr<QRPoint3D>* raw_pts, sptr<QRPolygon3D>* raw_polys,
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
        pointCodes.reserve(raw_pnt_cnt);
        pointCodes.setSize(raw_pnt_cnt);
        memset(pointCodes.getPureArray(), 0, sizeof(int32_t)*raw_pnt_cnt);

        point_arr.reserve(6*raw_pnt_cnt);
        poly_arr.reserve(6*raw_poly_cnt);   // todo hardcode
        // todo arrays must not expand!!!!!!!!
    }

    int32_t addRawPoint(size_t raw_ind);
    int32_t addPoint(float x, float y, float z);
    void addPoly(int32_t* arr, int size, size_t from_ind);

    sptr<QRPoint3D>* raw_points = nullptr;
    sptr<QRPolygon3D>* raw_polygons = nullptr;
    size_t raw_point_cnt, raw_polygon_cnt;

    Vector3D transZero;
    QuickMatrix matrix;
};

void QRMultVecMatrix(float*, float**);

#endif //BIG3DFLUFFY_QUICKRENDERDATA_H
