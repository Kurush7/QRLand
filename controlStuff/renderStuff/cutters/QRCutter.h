//
// Created by kurush on 09.08.2020.
//

#ifndef BIG3DFLUFFY_QRCUTTER_H
#define BIG3DFLUFFY_QRCUTTER_H

#include "QRDefines.h"
#include "objects/objects.h"

//  convex polygon
using renderPolygon = QRVector<Vector3D>;
struct renderTriangle {
    Vector3D p1, p2, p3;
};

enum cutResult {
   CUT_EMPTY = 0,
   CUT_FULL = 1,
   CUT_PARTIAL = 2
};


#endif //BIG3DFLUFFY_QRCUTTER_H
