//
// Created by kurush on 09.08.2020.
//

#ifndef BIG3DFLUFFY_QRPOLYPYRAMIDCUTTER_H
#define BIG3DFLUFFY_QRPOLYPYRAMIDCUTTER_H

#include "QRCutter.h"
// todo: copy points even if no need present! vectors are slow... use pure arrays

class Poly3DCutter {
public:
    cutResult cutPoly(const Vector3D* points, int size, renderPolygon& result);
    void setCutter(const Vector3D *planes, int cnt);
    // todo inside values must be > 0!!!!
private:
    cutResult innerCutter(renderPolygon& result);

    inline char getCode(const Vector3D &);
    inline Vector3D intersectionPoint(const Vector3D &p1, const Vector3D &p2, int cut_i);
    //inline float sideDist(float x, float y, int side);

    int Nw, Np;
    renderPolygon P, Q, cutter;
    Vector3D S, interP;
    bool interFlag;
};

#endif //BIG3DFLUFFY_QRPOLYPYRAMIDCUTTER_H
