//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_SIMPLECAMERA3D_H
#define BIG3DFLUFFY_SIMPLECAMERA3D_H

#include "../interfaces/QRCamera3D.h"
#include "SimplePoint3D.h"

class SimpleCamera3D: public QRCamera3D {
public:
    SimpleCamera3D(float w, float h, sptr<ProjectionImp>p, const Vector3D &origin,
                   const Vector3D &viewUp = Vector3D(), const Vector3D &bind = Vector3D());

    virtual bool isVisible(){return false;}


    virtual uptr<QRTransformer3D> getProjectionTransformer();

    // todo fuck: getVector() is tmp, returning ref
    virtual const Vector3D& getOrigin() const {return origin->getVector();}
    virtual void setOrigin(const Vector3D &p) { origin->setVector(p);}
    virtual const Vector3D& getBind() const {return origin->getBind();}
    virtual void setBind(const Vector3D &b) {origin->setBind(b);}

    // todo for class just to stay alive
    virtual void move(const Vector3D &move) {}
    virtual void scale(float sx, float sy) {}
    virtual void scale(float scale) {}
    virtual void rotate(const Vector3D &rotate) {}

private:
    Vector3D viewUpVector;
    sptr<QRPoint3D> origin;
};


#endif //BIG3DFLUFFY_SIMPLECAMERA3D_H
