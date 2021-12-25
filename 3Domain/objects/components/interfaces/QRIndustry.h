//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_QRINDUSTRY_H
#define BIG3DFLUFFY_QRINDUSTRY_H

#include "QRPoint3D.h"
#include "QREdge3D.h"
#include "QRCamera3D.h"
#include "QRPolygon3D.h"

class QRPoint3DCreator {
public:
    virtual uptr<QRPoint3D> create(const Vector3D &vec, const Vector3D &bind = Vector3D()) = 0;
};

class QREdge3DCreator {
public:
    virtual uptr<QREdge3D> create(sptr<QRPoint3D> start, sptr<QRPoint3D> end) = 0;
};

class QRPolygon3DCreator {
public:
    virtual uptr<QRPolygon3D> create(const QRVector<sptr<QRPoint3D>> &p) = 0;
    virtual uptr<QRPolygon3D> create(initializer_list<sptr<QRPoint3D>> lst) = 0;
};

class QRCamera3DCreator {
public:
    virtual uptr<QRCamera3D> create(float w, float h,
                                               const Vector3D &origin) = 0;
};


class QRObject3DFactory {
public:
    virtual uptr<QRPoint3D> createPoint(const Vector3D &vec, const Vector3D &bind = Vector3D()) = 0;
    virtual uptr<QREdge3D> createEdge(sptr<QRPoint3D> start,
                                                 sptr<QRPoint3D> end) = 0;
    virtual uptr<QRCamera3D> createCamera(float w, float h,
                                                     const Vector3D &origin) = 0;
};



#endif //BIG3DFLUFFY_QRINDUSTRY_H

