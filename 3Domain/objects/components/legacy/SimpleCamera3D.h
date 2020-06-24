//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_SIMPLECAMERA3D_H
#define BIG3DFLUFFY_SIMPLECAMERA3D_H

#include "../interfaces/QRCamera3D.h"
#include "SimplePoint3D.h"

class SimpleCamera3D: public QRCamera3D {
public:
    SimpleCamera3D(double w, double h, sptr<ProjectionImp>p, const Vector3D &origin,
                   const Vector3D &viewUp = Vector3D(), const Vector3D &bind = Vector3D());

    virtual bool isVisible(){return false;}


    virtual uptr<BaseTransformer3D> getProjectionTransformer();

    virtual const sptr<QRPoint3D> getOrigin() const {return origin;}
    virtual void setOrigin(sptr<QRPoint3D> p) { origin = p;}
    virtual const Vector3D& getBind() const {return origin->getBind();}
    virtual void setBind(const Vector3D &b) {origin->setBind(b);}

private:
    Vector3D viewUpVector;
    sptr<QRPoint3D> origin;
};


#endif //BIG3DFLUFFY_SIMPLECAMERA3D_H
