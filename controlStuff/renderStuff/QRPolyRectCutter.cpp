//
// Created by kurush on 30.06.2020.
//

#include "QRPolyRectCutter.h"

inline char checkVisibility(const Vector3D &a, const Vector3D &p1, const Vector3D &p2) {
    return -sign((a[0]-p1[0]) * (p2[1]-p1[1]) -
                 (a[1]-p1[1]) * (p2[0]-p1[0]));
}

inline bool checkIntersection(const Vector3D &a, const Vector3D &b, const Vector3D &w1, const Vector3D &w2) {
    char t1 = checkVisibility(a, w1, w2);
    char t2 = checkVisibility(b, w1, w2);
    return t1 * t2 < 0;
}

Vector3D intersectionPoint(const Vector3D &p1, const Vector3D &p2, const Vector3D &w1, const Vector3D &w2) {
    double pa = p1[1] - p2[1], wa = w1[1] - w2[1];
    double pb = p2[0] - p1[0], wb = w2[0] - w1[0];
    double pc = - pa * p1[0] - pb * p1[1];
    double wc = - wa * w1[0] - wb * w1[1];
    double zn = pa*wb -  pb*wa;
    Vector3D res;
    res[0] = -(pc*wb - pb*wc) / zn;
    res[1] = -(pa*wc - pc*wa) / zn;
    return res;
}

RenderPolygon cutPolyRect(const sptr<QRPolygon3D> &poly, double l, double r, double u, double d) {
    auto pointsBegin = poly->getPoints();
    RenderPolygon cutter({{l,d}, {r,d}, {r,u}, {l,u}, {l,d}}), P,Q;
    Vector3D S, interP;
    bool interFlag;

    int i = 0;
    for (auto it = pointsBegin; it; ++it) {
        P[i++] = it->get()->getVector();
    }
    int Np = P.getSize(), Nw = 5;

    for (int i = 0; i < Nw - 1; ++i) {
        for (int j = 0; j < Np; ++j) {
            if (j != 0) {
                interFlag = checkIntersection(S, P[j], cutter[i], cutter[i + 1]);
                if (interFlag) {
                    interP = intersectionPoint(S, P[j], cutter[i], cutter[i + 1]);
                    Q.push_back(interP);
                }
            }

            S = P[j];
            char visible = checkVisibility(S, cutter[i], cutter[i + 1]);
            if (visible >= 0)
                Q.push_back(S);
        }

        if (!Q.isEmpty()) {
            interFlag = checkIntersection(S, P[0], cutter[i], cutter[i + 1]);
            if (interFlag) {
                interP = intersectionPoint(S, P[0], cutter[i], cutter[i + 1]);
                Q.push_back(interP);
            }
        }
        P = Q;
        Q.clear();
        Np = P.getSize();
    }

   return P;
}

RenderPolygon cutPolyRect(const sptr<QRPolygon3D> &poly, const Vector3D &screenData) {
    double x = screenData[0], y = screenData[1], w = screenData[2], h = screenData[3];
    return cutPolyRect(poly, x - w/2, x + w/2, y + h/2, y - h/2);
}
