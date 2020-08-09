//
// Created by kurush on 30.06.2020.
//

#include "PolyRectCutter.h"


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

inline float PolyRectCutter::sideDist(float x, float y, int side) {
    return sqrt((x-cutter[side][0])*(x-cutter[side][0]) + (y-cutter[side][1])*(y-cutter[side][1]));
}

bool PolyRectCutter::cutPolyRect(const QRPolygon3D *poly, renderPolygon& result) {
    return cutPolyRect(poly->getPurePoints(), poly->getSize(), result);
}

bool PolyRectCutter::cutPolyRect(const sptr<QRPoint3D>* points, int size, renderPolygon& result) {
    Np = size;
    int code = 0, code_and=15;
    char c;
    P.clear();
    P.reserve(Np);
    result.clear();
    for (int i = 0; i < Np; ++i) {
        P[i] = points[i]->getVector();
        c = getCode(P[i][0], P[i][1]);
        code += c;
        code_and &= c;
    }
    P.setSize(Np);

    // fully visible and invisible
    if (code == 0) {result = P; return true; }
    else if (code_and != 0) return false;
    else return innerCutter(result);
}

bool PolyRectCutter::cutPolyRect(const Vector3D* points, int size, renderPolygon& result) {
    Np = size;
    int code = 0, code_and = 15;
    char c;
    P.clear();
    P.reserve(Np);
    result.clear();
    for (int i = 0; i < Np; ++i) {
        P[i] = points[i];
        c = getCode(P[i][0], P[i][1]);
        code += c;
        code_and &= c;
    }
    P.setSize(Np);

    // fully visible and invisible
    if (code == 0) {result = P; return true;}
    else if (code_and != 0) return false;
    else return innerCutter(result);
}

bool PolyRectCutter::innerCutter(renderPolygon& result) {
    char should_continue;
    Q.clear();
    int qsize=0;

    // consider big frame and small polygons: one side most times is enough.
    // so keep code running and move throug sides in nearest-to-random-poly-point order.
    int order[4] = {0,1,2,3};
    // todo full sort may be good
    if (sideDist(P[0][0], P[0][1], order[0]) > sideDist(P[0][0], P[0][1], order[1]))
        swap(order[0], order[1]);
    if (sideDist(P[0][0], P[0][1], order[0]) > sideDist(P[0][0], P[0][1], order[2]))
        swap(order[0], order[2]);
    if (sideDist(P[0][0], P[0][1], order[0]) > sideDist(P[0][0], P[0][1], order[3]))
        swap(order[0], order[3]);


    int i;
    for (int i0 = 0; i0 < Nw - 1; ++i0) {
        i = order[i0];
        should_continue = 0;
        for (int j = 0; j < Np; ++j) {
            if (j != 0) {
                interFlag = checkIntersection(S, P[j], cutter[i], cutter[i + 1]);
                if (interFlag) {
                    interP = intersectionPoint(S, P[j], i);
                    //if (!qsize || (abs(QRound(interP[0]) - QRound(Q[qsize-1][0])) > 0 ||
                    //               abs(QRound(interP[1]) - QRound(Q[qsize-1][1])) > 0))
                    Q.push_back(interP), qsize++;
                    should_continue |= getCode(interP[0], interP[1]);
                }
            }

            S = P[j];
            char visible = checkVisibility(S, cutter[i], cutter[i + 1]);
            if (visible >= 0) {
                // todo preventing from over-many-points on polygons.... bad for rasterizing if equal points
                //if (!qsize || (abs(QRound(S[0]) - QRound(Q[qsize-1][0])) > 0 ||
                //               abs(QRound(S[1]) - QRound(Q[qsize-1][1])) > 0))
                Q.push_back(S), qsize++;
                should_continue |= getCode(S[0], S[1]);
            }
        }

        if (!Q.isEmpty()) {
            interFlag = checkIntersection(S, P[0], cutter[i], cutter[i + 1]);
            if (interFlag) {
                interP = intersectionPoint(S, P[0], i);
                //if (!qsize || (abs(QRound(interP[0]) - QRound(Q[qsize-1][0])) > 0 ||
                //               abs(QRound(interP[1]) - QRound(Q[qsize-1][1])) > 0))
                Q.push_back(interP), qsize++;
                should_continue |= getCode(interP[0], interP[1]);
            }
        }
        P = Q;
        Q.clear();
        qsize=0;
        Np = P.getSize();
        if (!should_continue)  break;
    }
    result = P;
    return true;
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