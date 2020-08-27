//
// Created by kurush on 19.08.2020.
//

#ifndef BIG3DFLUFFY_GRAHAMCONVEX_H
#define BIG3DFLUFFY_GRAHAMCONVEX_H

#include "math/QRMath.h"
#include "containers/QRContainers.h"

#include <algorithm>
#include <vector>

QRVector<Vector3D> makeConvex(const QRVector<Vector3D> &polygon);


#endif //BIG3DFLUFFY_GRAHAMCONVEX_H
