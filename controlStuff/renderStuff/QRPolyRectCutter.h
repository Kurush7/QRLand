//
// Created by kurush on 30.06.2020.
//

#ifndef BIG3DFLUFFY_QRPOLYRECTCUTTER_H
#define BIG3DFLUFFY_QRPOLYRECTCUTTER_H

#include "QRDefines.h"
#include "objects/objects.h"

//  convex polygon
using RenderPolygon = QRVector<Vector3D>;

// cuts convex 2d-poly (ignores oZ) with rectangle, returns NEW cut convex polygon

RenderPolygon cutPolyRect(const QRPolygon3D *poly, float l, float r, float u, float d);
RenderPolygon cutPolyRect(const QRPolygon3D *poly, const Vector3D &screenData);


#endif //BIG3DFLUFFY_QRPOLYRECTCUTTER_H
