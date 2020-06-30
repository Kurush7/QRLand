//
// Created by kurush on 30.06.2020.
//

#ifndef BIG3DFLUFFY_QRPOLYRECTCUTTER_H
#define BIG3DFLUFFY_QRPOLYRECTCUTTER_H

#include "QRDefines.h"
#include "objects/objects.h"

using RenderPolygon = QRVector<Vector3D>;
RenderPolygon cutPolyRect(const sptr<QRPolygon3D> &poly, double l, double r, double u, double d);
RenderPolygon cutPolyRect(const sptr<QRPolygon3D> &poly, const Vector3D &screenData) {
    double x = screenData[0], y = screenData[1], w = screenData[2], h = screenData[3];
    return cutPolyRect(poly, x - w/2, x + w/2, y + h/2, y - h/2);
}


#endif //BIG3DFLUFFY_QRPOLYRECTCUTTER_H
