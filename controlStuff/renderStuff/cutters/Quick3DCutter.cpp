//
// Created by kurush on 29.08.2020.
//

#include "Quick3DCutter.h"

inline char checkVisibility(float *a, float *plane) {
    return sign(a[0]*plane[0] + a[1]*plane[1] + a[2]*plane[2] + plane[3]);
}

inline bool checkIntersection(float *a, float *b, float *plane) {
    return checkVisibility(a, plane) * checkVisibility(b, plane) < 0;
}

inline void Quick3DCutter::intersectionPoint(float *p1, float *p2, int cut_i) {
    float x = (p1[0]*cutter[cut_i][0] + p1[1]*cutter[cut_i][1] + p1[2]*cutter[cut_i][2] + cutter[cut_i][3]) /
            ((p2[0]-p1[0])*cutter[cut_i][0] + (p2[1]-p1[1])*cutter[cut_i][1] + (p2[2]-p1[2])*cutter[cut_i][2]);
    interP[0] = p1[0] - x*(p2[0]-p1[0]);
    interP[1] = p1[1] - x*(p2[1]-p1[1]);
    interP[2] = p1[2] - x*(p2[2]-p1[2]);
}

inline char Quick3DCutter::getCode(float *v) {
    char code = 0;
    for (char i = 0, power=1; i < Nw; ++i, power*=2)
        if (v[0]*cutter[i][0] + v[1]*cutter[i][1] + v[2]*cutter[i][2] + cutter[i][3] < -QREPS)
            code += power;
    return code;
}

mutex dataMutex;
void Quick3DCutter::cutPoly(size_t ind) {
    Np = data.raw_polygons[ind]->getSize();
    int code = 0, code_and = (1<<Nw)-1;
    int c;
    size_t x;
    P.reserve(Np);
    P.setSize(Np);

    auto indexes = data.raw_polygons[ind]->getPurePointIndexes();
    QRVector<int32_t> &pointCodes = data.pointCodes;

    for (int i = 0; i < Np; ++i) {
        c = pointCodes[indexes[i]];
        if (c == 0) {
            //dataMutex.lock();
            x = data.addRawPoint(indexes[i]);
            data.matrix.mult(data.points[x]);
            c = getCode(data.points[x]);
            dataMutex.lock();
            pointCodes[indexes[i]] = x*100 + c;
            dataMutex.unlock();
            P[i] = x;
        }
        else {
            P[i] = c / 100;
            c = c % 100;
        }
        code += c;
        code_and &= c;
    }

    // fully visible and invisible
    if (code == 0) {
        data.addPoly(P.getPureArray(), P.getSize(), ind);
    }
    else if (code_and != 0) return;
    else innerCutter(ind);
}

void Quick3DCutter::innerCutter(size_t ind) {
    Q.clear();
    int32_t x;
    char prevCode;
    for (int i = 0; i < Nw; ++i) {
        for (int j = 0; j < Np; ++j) {
            if (j != 0) {
                interFlag = checkIntersection(data.points[S], data.points[P[j]], cutter[i]);
                if (interFlag) {
                    intersectionPoint(data.points[S], data.points[P[j]], i);
                    x = data.addPoint(interP[0], interP[1], interP[2]);
                    Q.push_back(x);
                }
            }

            S = P[j];
            char visible = checkVisibility(data.points[S], cutter[i]);
            if (visible >= 0)
                Q.push_back(S);
        }

        if (!Q.isEmpty()) {
            interFlag = checkIntersection(data.points[S], data.points[P[0]], cutter[i]);
            if (interFlag) {
                intersectionPoint(data.points[S], data.points[P[0]], i);
                x = data.addPoint(interP[0], interP[1], interP[2]);
                Q.push_back(x);
            }
        }
        P.swap(Q);
        Q.clear();
        Np = P.getSize();
    }
    if (!P.isEmpty()) {
        for (auto p: P)
            cout << p << '\n';
        cout << '\n';
        data.addPoly(P.getPureArray(), Np, ind);
    }
}

void Quick3DCutter::setCutter(const Vector3D *planes, int cnt) {
    if (cutter) {
        for (int i = 0; i < cutterSize; ++i)
            delete[] cutter[i];
        delete[] cutter;
    }
    cutter = new float*[cnt];
    for (int i = 0; i < cnt; ++i) {
        cutter[i] = new float[4];
        cutter[i][0] = planes[i][0];
        cutter[i][1] = planes[i][1];
        cutter[i][2] = planes[i][2];
        cutter[i][3] = planes[i][3];
    }
    cutterSize = cnt;
    Nw = cnt;
}