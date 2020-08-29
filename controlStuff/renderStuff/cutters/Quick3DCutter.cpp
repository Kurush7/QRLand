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
    Vector3D v1(p1[0], p1[1], p1[2], 1), v2(p2[0], p2[1], p2[2], 1),
    cut(cutter[cut_i][0], cutter[cut_i][1], cutter[cut_i][2], cutter[cut_i][3]);
    Vector3D p =  v1 - scalar(v1, cut) / scalar3(v2-v1, cut) * (v2-v1);
    interP[0] = p[0];
    interP[1] = p[1];
    interP[2] = p[2];
    return;

    float x = (p1[0]*cutter[cut_i][0] + p1[1]*cutter[cut_i][1] + p1[2]*cutter[cut_i][2] + cutter[cut_i][3]) /
            ((p2[0]-p1[0])*cutter[cut_i][0] + (p2[1]-p1[1])*cutter[cut_i][1] + (p2[2]-p1[2])*cutter[cut_i][2]);
    interP[0] = p1[0] - x*(p2[0]-p1[0]);
    interP[1] = p1[1] - x*(p2[1]-p1[1]);
    interP[2] = p1[2] - x*(p2[2]-p1[2]);
}

inline char Quick3DCutter::getCode(float *v) {
    char code = 0, power = 1;
    for (int i = 0; i < Nw; ++i) {
        if (v[0]*cutter[i][0] + v[1]*cutter[i][1] +
        v[2]*cutter[i][2] + cutter[i][3] < -QREPS) code += power;
        power *= 2;
    }
    return code;
}

void Quick3DCutter::cutPoly(size_t ind) {
    Np = data.raw_polygons[ind]->getSize();
    int code = 0, code_and = pow(2, Nw)-1;
    char c;
    P.clear();
    P.reserve(Np);
    P.setSize(Np);

    auto indexes = data.raw_polygons[ind]->getPurePointIndexes();
    for (int i = 0; i < Np; ++i) {
        if (data.pointCodes[indexes[i]] == 0) {
            size_t x = data.addRawPoint(indexes[i]);
            data.matrix.mult(data.points[x]);
            data.pointCodes[indexes[i]] += getCode(data.points[x]);
            //cout << "* " << x << ":::  " << data.points[x][0] << ' ' << data.points[x][1] << ' ' <<data.points[x][2] << "\n";
            P[i] = x;
        }
        else {
            P[i] = data.pointCodes[indexes[i]] / 100;
        }
        c = data.pointCodes[indexes[i]] % 100;
        //cout << indexes[i]<< ": " << P[i] << ' ' << data.pointCodes[indexes[i]] << " :: "<< data.points[P[i]][0] << ' ' << data.points[P[i]][1] << ' '
        //<< data.points[P[i]][2] <<  " => " << (int)c << '\n';
        code += c;
        code_and &= c;
    }

    // fully visible and invisible
    if (code == 0) {
        data.addPoly(P.getPureArray(), P.getSize(), ind);
    }
    else if (code_and != 0) {
        return;
    }
    else innerCutter(ind);
}

void Quick3DCutter::innerCutter(size_t ind) {
    Q.clear();
    int32_t x;
    for (int i = 0; i < Nw; ++i) {
        for (int j = 0; j < Np; ++j) {
            if (j != 0) {
                interFlag = checkIntersection(data.points[S], data.points[P[j]], cutter[i]);
                // todo  use &and instead (above)
                if (interFlag) {
                    intersectionPoint(data.points[S], data.points[P[j]], i);
                    cout << "interp: " << interP[0] << ' ' << interP[1] << ' ' << interP[2] << '\n';
                    x = data.addPoint(interP[0], interP[1], interP[2]);
                    data.pointCodes[x] += getCode(interP);
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
            // todo  use & instead (above)
            if (interFlag) {
                intersectionPoint(data.points[S], data.points[P[0]], i);
                cout << "interp: " << interP[0] << ' ' << interP[1] << ' ' << interP[2] << '\n';
                x = data.addPoint(interP[0], interP[1], interP[2]);
                data.pointCodes[x] += getCode(interP);
                Q.push_back(x);
            }
        }
        P = Q;  // todo avoid copying
        Q.clear();
        Np = P.getSize();
    }
    if (!P.isEmpty())
        data.addPoly(P.getPureArray(), Np, ind);
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