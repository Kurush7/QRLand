//
// Created by kurush on 19.08.2020.
//

#include "LineIntersection.h"

#define det(a,b,c,d)  (a*d-b*c)

Vector3D intersectLines(const Vector3D &m, const Vector3D &n, char *status) {
    float zn = det(m[0], m[1], n[0], n[1]);
    if (fabs (zn) < QREPS)
    {
        if (status) {
            if (fabs(det (m[2], m[1], n[2], n[1])) < QREPS &&
                fabs(det (m[0], m[2], n[0], n[2])) < QREPS)
                *status = 2;
            else *status = 0;
        }
        return ZeroVector;
    }
    else {
        float x = - det (m[2], m[1], n[2], n[1]) / zn;
        float y = - det (m[0], m[2], n[0], n[2]) / zn;
        if (status)
            *status = 1;
        return Vector3D(x, y, 0);
    }
}