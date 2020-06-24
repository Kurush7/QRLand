//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_QRINDUSTRY_H
#define BIG3DFLUFFY_QRINDUSTRY_H

#include "QRPoint3D.h"
#include "QREdge3D.h"
#include "QRCamera3D.h"

class BaseQRPoint3DCreator {
public:
    virtual std::unique_ptr<QRPoint3D> create(const Vector3D &vec, const Vector3D &bind = Vector3D()) = 0;
};

class BaseEdge3DCreator {
public:
    virtual std::unique_ptr<QREdge3D> create(std::shared_ptr<QRPoint3D> start, std::shared_ptr<QRPoint3D> end) = 0;
};


class BaseCamera3DCreator {
public:
    virtual std::unique_ptr<QRCamera3D> create(double w, double h,
                                               const Vector3D &origin) = 0;
};


class AbstractObject3DFactory {
public:
    virtual std::unique_ptr<QRPoint3D> createPoint(const Vector3D &vec, const Vector3D &bind = Vector3D()) = 0;
    virtual std::unique_ptr<QREdge3D> createEdge(std::shared_ptr<QRPoint3D> start,
                                                 std::shared_ptr<QRPoint3D> end) = 0;
    virtual std::unique_ptr<QRCamera3D> createCamera(double w, double h,
                                                     const Vector3D &origin) = 0;
};



#endif //BIG3DFLUFFY_QRINDUSTRY_H

