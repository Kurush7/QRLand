//
// Created by kurush on 30.06.2020.
//

#ifndef BIG3DFLUFFY_POLYRECTCUTTER_H
#define BIG3DFLUFFY_POLYRECTCUTTER_H

#include "QRCutter.h"
// todo: copy points even if no need present! vectors are slow... use pure arrays


class PolyRectCutter {
public:
    bool cutPolyRect(const QRPolygon3D *poly, renderPolygon& result);
    bool cutPolyRect(const sptr<QRPoint3D>* points, int size, renderPolygon& result);
    bool cutPolyRect(const Vector3D* points, int size, renderPolygon& result);
    void setCutter(float l, float r, float u, float d);
    void setCutter(const Vector3D &screenData);
private:
    bool innerCutter(renderPolygon& result);

    inline char getCode(float x, float y);
    inline Vector3D intersectionPoint(const Vector3D &p1, const Vector3D &p2, int cut_i);
    inline float sideDist(float x, float y, int side);

    //
    int Nw = 5, Np;
    renderPolygon cutter, P, Q;
    float cut_a[4], cut_b[4], cut_c[4];
    Vector3D S, interP;
    bool interFlag;
};

#endif //BIG3DFLUFFY_POLYRECTCUTTER_H
