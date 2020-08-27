//
// Created by kurush on 19.08.2020.
//

#ifndef BIG3DFLUFFY_VORONOIDIAGRAM_H
#define BIG3DFLUFFY_VORONOIDIAGRAM_H

#include "containers/QRContainers.h"
#include "math/QRMath.h"
#include "../figures/QRFrame2D.h"

#include "LineIntersection.h"
#include "GrahamConvex.h"

// all points must be inside the rectangle!
// todo add check for in-rectangle
QRVector<sptr<QRFrame2D>> buildVoronoiDiagramOnRect(
        double left, double right, double down, double up,
        const QRVector<Vector3D> &points);


#endif //BIG3DFLUFFY_VORONOIDIAGRAM_H
