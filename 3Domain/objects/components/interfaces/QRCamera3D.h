//
// Created by kurush on 30.05.2020.
//

#ifndef BIG3DFLUFFY_CAMERA_H
#define BIG3DFLUFFY_CAMERA_H

#include "objects/QRObject.h"
#include "QRPoint3D.h"
#include "QRProjection.h"

class Camera3DMemento;

class QRCamera3D: public QRObject3D {
public:
    QRCamera3D(double w, double h, sptr<ProjectionImp>pr): width(w), height(h), projector(pr){
        p = sptr<QRCamera3D>(this, [](void *ptr){});}
    ~QRCamera3D() {p.reset();}

    virtual void acceptVisitor(sptr<QRVisitor> visitor) {visitor->visitCamera3D(p);}

    virtual uptr<QRMemento> save();
    virtual uptr<BaseTransformer3D> getProjectionTransformer() = 0;

    virtual bool operator==(const QRCamera3D &b) const {return getOrigin() == b.getOrigin() &&
                                                                 getBind() == b.getBind() && width == b.width &&
                                                                 height == b.height;}
    virtual QRCamera3D& operator=(const QRCamera3D &p) {}

// getters & setters
    double getWidth() const;
    void setWidth(double width);
    double getHeight() const;
    void setHeight(double height);

    virtual const sptr<QRPoint3D> getOrigin() const = 0;
    virtual void setOrigin(sptr<QRPoint3D>) = 0;

    virtual const Vector3D& getBind() const = 0;
    virtual void setBind(const Vector3D&) = 0;

    virtual const sptr<ProjectionImp> getProjector() const {return projector;}
    virtual const void setProjector(sptr<ProjectionImp> p) {projector = p;}

    wptr<QRCamera3D> getPointer() {return p;}

private:
    sptr<QRCamera3D> p;
protected:
    sptr<ProjectionImp> projector;
    double width, height;
};

#endif //BIG3DFLUFFY_QRCAMERA3D_H
