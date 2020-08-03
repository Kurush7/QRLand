//
// Created by kurush on 30.06.2020.
//

#include "QRPolyRectCutter.h"

inline char checkVisibility(const Vector3D &a, const Vector3D &p1, const Vector3D &p2) {
    return -sign((a[0]-p1[0]) * (p2[1]-p1[1]) -
                 (a[1]-p1[1]) * (p2[0]-p1[0]));
}

inline bool checkIntersection(const Vector3D &a, const Vector3D &b, const Vector3D &w1, const Vector3D &w2) {
    //return checkVisibility(a, w1, w2) * checkVisibility(b, w1, w2) < 0;
    return sign((a[0]-w1[0]) * (w2[1]-w1[1]) - (a[1]-w1[1]) * (w2[0]-w1[0])) *
            sign((b[0]-w1[0]) * (w2[1]-w1[1]) - (b[1]-w1[1]) * (w2[0]-w1[0])) < 0;
}


inline Vector3D PolyRectCutter::intersectionPoint(const Vector3D &p1, const Vector3D &p2, int cut_i) {
    float pa = p1[1] - p2[1], pb = p2[0] - p1[0];
    float pc = - pa * p1[0] - pb * p1[1];
    float zn = pa*cut_b[cut_i] -  pb*cut_a[cut_i];
    Vector3D res;
    res[0] = -(pc*cut_b[cut_i] - pb*cut_c[cut_i]) / zn;
    res[1] = -(pa*cut_c[cut_i] - pc*cut_a[cut_i]) / zn;
    if (fabs(p2[0] - p1[0]) > QREPS)
        res[2] = p1[2] + (p2[2] - p1[2]) * (res[0] - p1[0]) / (p2[0] - p1[0]);
    else
        res[2] = p1[2] + (p2[2] - p1[2]) * (res[1] - p1[1]) / (p2[1] - p1[1]);
    return res;
}

inline char PolyRectCutter::getCode(float x, float y) {
    char code = 0;
    if (x < cutter[0][0]) code += 1;
    if (x > cutter[1][0]) code += 2;
    if (y < cutter[0][1]) code += 4;
    if (y > cutter[2][1]) code += 8;
    return code;
}

renderPolygon PolyRectCutter::cutPolyRect(const QRPolygon3D *poly) {
    renderPolygon P;
    auto poly_pts = poly->getPurePoints();
    Np = poly->getSize();
    int code = 0;
    for (int i = 0; i < Np; ++i) {
        P.push_back(poly_pts[i]->getVector());
        code += getCode(P[i][0], P[i][1]);
    }
    if (code == 0) return P;

    Q.clear();
    for (int i = 0; i < Nw - 1; ++i) {
        for (int j = 0; j < Np; ++j) {
            if (j != 0) {
                interFlag = checkIntersection(S, P[j], cutter[i], cutter[i + 1]);
                if (interFlag) {
                    interP = intersectionPoint(S, P[j], i);
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
                interP = intersectionPoint(S, P[0], i);
                Q.push_back(interP);
            }
        }
        P = Q;
        Q.clear();
        Np = P.getSize();
    }

   return P;
}

void PolyRectCutter::setCutter(const Vector3D &screenData) {
    float x = screenData[0], y = screenData[1], w = screenData[2], h = screenData[3];
    setCutter(x - w/2, x + w/2, y + h/2, y - h/2);
}
void PolyRectCutter::setCutter(float l, float r, float u, float d) {
    cutter.clear();
    cutter.push_back({l, d});
    cutter.push_back({r, d});
    cutter.push_back({r, u});
    cutter.push_back({l, u});
    cutter.push_back({l, d});

    int i1;
    for (int i = 0; i < 4; ++i) {
        i1=(i+1)%4;
        cut_a[i] = cutter[i][1] - cutter[i1][1];
        cut_b[i] = cutter[i1][0] - cutter[i][0];
        cut_c[i] = -cut_a[i]*cutter[i][0] - cut_b[i]*cutter[i][1];
    }

}