//
// Created by kurush on 28.06.2020.
//

#ifndef BIG3DFLUFFY_CAMERA3D_H
#define BIG3DFLUFFY_CAMERA3D_H

#include "../interfaces/QRCamera3D.h"
#include "math/Transformer3D.h"

/**
 this camera holds a center point, vector from perspective center to the 'view-plane' center,
 and a ViewUpVector. so, no far plane is supported yet for cutting
 */

class Camera3D: public QRCamera3D {
public:
    Camera3D(double w, double h, sptr<ProjectionImp>p, const Vector3D &origin,
                   const Vector3D &viewUp = Vector3D(), const Vector3D &bind = Vector3D());

    virtual bool isVisible(){return false;}

    virtual void move(const Vector3D &move);
    virtual void scale(double sx, double sy);
    virtual void scale(double scale);
    virtual void rotate(const Vector3D &rotate);

    virtual uptr<QRTransformer3D> getProjectionTransformer();

    virtual const sptr<QRPoint3D> getOrigin() const {return origin;}
    virtual void setOrigin(sptr<QRPoint3D> p) { origin = p;}

    // todo not all params considered
    virtual bool operator==(const QRCamera3D &b) const {return origin == b.getOrigin() &&
                                                               width == b.getWidth() &&
                                                               height == b.getHeight();}
private:
    Vector3D viewUpVector, planeVector;
    sptr<QRPoint3D> origin;
};


#endif //BIG3DFLUFFY_CAMERA3D_H
