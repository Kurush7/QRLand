//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_QRVISITORINTERFACE_H
#define BIG3DFLUFFY_QRVISITORINTERFACE_H


#include <memory>
#include "../../QRDefines.h"

class QRPoint3D;
class QREdge3D;
class QRCamera3D;
class QRFrame3D;
class QRPolygon3D;

class QRVisitor {
public:
    QRVisitor() { p = sptr<QRVisitor>(this, [](void *ptr){});}
    virtual void visitPoint3D(sptr<QRPoint3D> point) = 0;
    virtual void visitEdge3D(sptr<QREdge3D> edge) = 0;
    virtual void visitCamera3D(sptr<QRCamera3D> camera) = 0;
    virtual void visitFrame3D(sptr<QRFrame3D> frame);

    // todo make abstract after disconnecting legacy visitors
    virtual void visitPolygon3D(sptr<QRPolygon3D> poly) {}

protected:
    sptr<QRVisitor> p;
};

#endif //BIG3DFLUFFY_QRVISITORINTERFACE_H
