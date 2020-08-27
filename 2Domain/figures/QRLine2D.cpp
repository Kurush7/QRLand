//
// Created by kurush on 19.08.2020.
//

#include "QRLine2D.h"

bool operator < (const QRLine2D &p, const QRLine2D& q) {
    // todo needed for mapping of unoriented edges.... in plate-mountain-tool
    Vector3D pa= min(p.a, p.b), pb = max(p.a, p.b);
    Vector3D qa= min(q.a, q.b), qb = max(q.a, q.b);
    if (pa < qa) return true;
    else if (qa < pa) return false;
    else return pb < qb;
}