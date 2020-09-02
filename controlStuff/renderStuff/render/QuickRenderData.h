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

    void mult(float*) const;
    void projMult(float*) const;
    void addPerspective(const Matrix3D &m) {
        dot[0] = m[3][0], dot[1] = m[3][1], dot[2] = m[3][2];
    }

    float **matrix = nullptr;
    float *tmp = new float[3];
    float *add = new float[3], *dot = new float[3];
};

class QuickRenderData {
public:
    ~QuickRenderData() {}
    explicit QuickRenderData(QRVector<int32_t> &pointCodes): pointCodes(pointCodes) {}

    int32_t *poly_arr = nullptr;
    int32_t polySize = 0;
    QRVector<int32_t*> polygons;
    QRVector<char> polygonSize;
    QRVector<int32_t> rawPolyMap;
    QRVector<Vector3D> normals;

    float *point_arr = nullptr;  // by triples of {x,y,z}
    float **points, **myPoints;
    int32_t points_offset;
    QRVector<int32_t> &pointCodes;  // 0 if point not created, code/100 is mapped index  // todo bug for 0'th point to be parsed twice if it's visible
    int32_t pointsSize = 0;

    void init(const Matrix3D &m, sptr<QRPoint3D>* raw_pts, sptr<QRPolygon3D>* raw_polys,
              size_t raw_pnt_cnt, size_t raw_poly_cnt,
              int32_t *polyArr, float*pointArr, float** pts, float** myPts, int32_t offset) {
        transZero = m * ZeroVector;

        matrix = m;

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
        // todo control!!! arrays must not expand!!!!!!!!
    }

    int32_t addRawPoint(size_t raw_ind);
    int32_t addPoint(float x, float y, float z);
    void addPoly(int32_t* arr, int size, size_t from_ind);

    sptr<QRPoint3D>* raw_points = nullptr;
    sptr<QRPolygon3D>* raw_polygons = nullptr;

    Vector3D transZero;
    QuickMatrix matrix;
};

class QuickRenderMetaData {
public:
    QuickRenderMetaData(int thread_cnt = RENDER_THREAD_CNT): thread_cnt(thread_cnt) {
        for (int i = 0; i < thread_cnt; ++i)
            data.push_back(new QuickRenderData(pointCodes));
    }
    ~QuickRenderMetaData() {
        delete[] point_arr;
        delete[] poly_arr;
        for (int i = 0; i < data.getSize(); ++i) delete data[i];
    }

    void init(const Matrix3D &m, sptr<QRPoint3D>* raw_pts, sptr<QRPolygon3D>* raw_polys,
              size_t raw_pnt_cnt, size_t raw_poly_cnt) {
        matrix = m;

        int secure_coef = 8;
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
        for (int i = 0; i < thread_cnt; ++i) {
            data[i]->init(m, raw_pts, raw_polys, raw_pnt_cnt, raw_poly_cnt, &poly_arr[i*polyStep],
                    &point_arr[i*pointStep], points, &points[i*ptStep], i*ptStep);
        }
    }


    QRVector<QuickRenderData*> data;
    int32_t *poly_arr = nullptr;
    size_t poly_arr_size = 0;

    float *point_arr = nullptr;  // by triples of {x,y,z}
    float** points;
    size_t point_arr_size = 0, points_size = 0;
    // todo bug for 0'th point to be parsed twice if it's visible
    QRVector<int32_t> pointCodes;  // 0 if point not created, code/100 is mapped index

    int thread_cnt;
    QuickMatrix matrix;
};



#endif //BIG3DFLUFFY_QUICKRENDERDATA_H
