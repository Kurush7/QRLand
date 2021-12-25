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
#include "QRConstants.h"


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

    int32_t addRawPoint(size_t raw_ind);
    int32_t addPoint(float x, float y, float z);
    void addPoly(int32_t* arr, int size, size_t from_ind);

    sptr<QRPoint3D>* raw_points = nullptr;
    sptr<QRPolygon3D>* raw_polygons = nullptr;
    QuickMatrix matrix, modelMatrix;

    void init(const Matrix3D &m, const Matrix3D &modelMatrix, sptr<QRPoint3D>* raw_pts, sptr<QRPolygon3D>* raw_polys,
              size_t raw_pnt_cnt, size_t raw_poly_cnt,
              int32_t *polyArr, float*pointArr, float** pts, float** myPts, int32_t offset);
};

class QuickRenderMetaData {
public:
    QuickRenderMetaData(int thread_cnt = RENDER_THREAD_CNT): thread_cnt(thread_cnt) {
        for (int i = 0; i < thread_cnt; ++i)
            data.push_back(new QuickRenderData(pointCodes));

            poly_arr = new int32_t[minQuickRenderDataBufferSize], poly_arr_size = minQuickRenderDataBufferSize;

            point_arr = new float[minQuickRenderDataBufferSize], point_arr_size = minQuickRenderDataBufferSize;
            points = new float*[minQuickRenderDataBufferSize], points_size = point_arr_size/3;
    }
    ~QuickRenderMetaData() {
        delete[] point_arr;
        delete[] poly_arr;
        for (int i = 0; i < data.getSize(); ++i) delete data[i];
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

    void init(const Matrix3D &m, const Matrix3D &modelMatrix, sptr<QRPoint3D>* raw_pts, sptr<QRPolygon3D>* raw_polys,
              size_t raw_pnt_cnt, size_t raw_poly_cnt);
};



#endif //BIG3DFLUFFY_QUICKRENDERDATA_H
