//
// Created by kurush on 30.06.2020.
//

#ifndef BIG3DFLUFFY_QRPOLYRECTCUTTER_H
#define BIG3DFLUFFY_QRPOLYRECTCUTTER_H

#include "QRDefines.h"
#include "objects/objects.h"

//  convex polygon
using renderPolygon = QRVector<Vector3D>;
struct renderTriangle {
    Vector3D p1, p2, p3;
};


class PolyRectCutter {
public:
    renderPolygon cutPolyRect(const QRPolygon3D *poly);
    void setCutter(float l, float r, float u, float d);
    void setCutter(const Vector3D &screenData);
private:
    inline char getCode(float x, float y);
    inline Vector3D intersectionPoint(const Vector3D &p1, const Vector3D &p2, int cut_i);

    int Nw = 5;
    renderPolygon cutter;
    float cut_a[4], cut_b[4], cut_c[4];
    renderPolygon P, Q;
    Vector3D S, interP;
    bool interFlag;
};

#endif //BIG3DFLUFFY_QRPOLYRECTCUTTER_H
