//
// Created by kurush on 09.08.2020.
//

#include "Poly3DCutter.h"

inline char checkVisibility(const Vector3D &a, const Vector3D &plane) {
    return sign(scalar(a, plane));
}

inline bool checkIntersection(const Vector3D &a, const Vector3D &b, const Vector3D &plane) {
    return checkVisibility(a, plane) * checkVisibility(b, plane) < 0;
}

inline Vector3D Poly3DCutter::intersectionPoint(const Vector3D &p1, const Vector3D &p2, int cut_i) {
    Vector3D p =  p1 - scalar(p1, cutter[cut_i]) / scalar3(p2-p1, cutter[cut_i]) * (p2-p1);
    p[3] = 1;
    return p;
}

inline char Poly3DCutter::getCode(const Vector3D &v) {
    // todo v[3] must be 1!!!!
    // todo char's limits are 127-> no more than 7 planes!
    char code = 0;
    char power = 1;
    for (int i = 0; i < Nw; ++i) {
        if (scalar(v, cutter[i]) < 0) code += power;
        power *= 2;
    }
    return code;
}
/*
inline float Poly3DCutter::sideDist(float x, float y, int side) {
    return sqrt((x-cutter[side][0])*(x-cutter[side][0]) + (y-cutter[side][1])*(y-cutter[side][1]));
}*/

cutResult Poly3DCutter::cutPoly(const Vector3D* points, int size, renderPolygon& result) {
    Np = size;
    int code = 0, code_and = pow(2, Nw)-1;
    char c;
    P.clear();
    P.reserve(Np);
    result.clear();
    for (int i = 0; i < Np; ++i) {
        P[i] = points[i];
        P[i][3] = 1;
        c = getCode(P[i]);
        code += c;
        code_and &= c;
    }
    P.setSize(Np);

    // fully visible and invisible
    if (code == 0) {result = P; return CUT_FULL;}
    else if (code_and != 0) return CUT_EMPTY;
    else return innerCutter(result);
}

cutResult Poly3DCutter::innerCutter(renderPolygon& result) {
    //char should_continue;
    Q.clear();
    int qsize=0;
    // consider big frame and small polygons: one side most times is enough.
    // so keep code running and move throug sides in nearest-to-random-poly-point order.

    for (int i = 0; i < Nw; ++i) {
        //should_continue = 0;
        for (int j = 0; j < Np; ++j) {
            if (j != 0) {
                interFlag = checkIntersection(S, P[j], cutter[i]);
                if (interFlag) {
                    interP = intersectionPoint(S, P[j], i);
                    Q.push_back(interP), qsize++;
                    //should_continue |= getCode(interP[0], interP[1]);
                }
            }

            S = P[j];
            char visible = checkVisibility(S, cutter[i]);
            if (visible >= 0) {
                Q.push_back(S), qsize++;
                //should_continue |= getCode(S[0], S[1]);
            }
        }

        if (!Q.isEmpty()) {
            interFlag = checkIntersection(S, P[0], cutter[i]);
            if (interFlag) {
                interP = intersectionPoint(S, P[0], i);
                Q.push_back(interP), qsize++;
                //should_continue |= getCode(interP[0], interP[1]);
            }
        }
        P = Q;
        Q.clear();
        qsize=0;
        Np = P.getSize();
        //if (!should_continue)  break;
    }
    result = P;
    return CUT_PARTIAL; // todo not always true. full possible.... or empty
}

void Poly3DCutter::setCutter(const Vector3D *planes, int cnt) {
    cutter.clear();
    for (int i = 0; i < cnt; ++i)
        cutter[i] = planes[i];
    Nw = cnt;
}