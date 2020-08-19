//
// Created by kurush on 19.08.2020.
//

#ifndef BIG3DFLUFFY_LINEINTERSECTION_H
#define BIG3DFLUFFY_LINEINTERSECTION_H

#include "../figures/QRLine2D.h"

// status: 1 if 1point, 2 if many points, 0 if none
// line m is ax+by+c, where: a=m[0], b = m[1], c = m[2]
Vector3D intersectLines(const Vector3D &m, const Vector3D &n, char *status=nullptr);

#endif //BIG3DFLUFFY_LINEINTERSECTION_H
