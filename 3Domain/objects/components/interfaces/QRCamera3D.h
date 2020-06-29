//
// Created by kurush on 30.05.2020.
//

#ifndef BIG3DFLUFFY_CAMERA_H
#define BIG3DFLUFFY_CAMERA_H

#include "objects/QRObject.h"
#include "QRPoint3D.h"
#include "QRProjection.h"

/**
 alike point, which is not holding it's scene position, camera has this info, for it
 is necessary when forming proection matrix
 */
class QRCamera3D: public QRObject3D {
public:
    QRCamera3D(double w, double h, sptr<ProjectionImp>pr): width(w), height(h), projector(pr){
        p = sptr<QRCamera3D>(this, [](void *ptr){});}
    ~QRCamera3D() {p.reset();}

    virtual void acceptVisitor(const sptr<QRVisitor>& visitor) {visitor->visitCamera3D(p);}
    virtual uptr<QRMemento> save();

    virtual uptr<QRTransformer3D> getProjectionTransformer() = 0;

    virtual void move(const Vector3D &move) = 0;
    virtual void scale(double sx, double sy) = 0;
    virtual void scale(double scale) = 0;
    virtual void rotate(const Vector3D &rotate) = 0;

    // todo deprecated. delete
    virtual const Vector3D& getBind() const {}
    virtual void setBind(const Vector3D &b) {}

    // todo isVisibleSphere
    // todo to camera coords?

    // todo
    //virtual bool operator==(const QRCamera3D &b) const = 0;
    //virtual QRCamera3D& operator=(const QRCamera3D &p) {}

// getters & setters
    // todo cutting planes.......
    double getWidth() const {return width;}
    void setWidth(double w) {width = w;}
    double getHeight() const {return height;}
    void setHeight(double h) {h = height;}

    virtual const Vector3D& getOrigin() const = 0;
    virtual void setOrigin(const Vector3D&) = 0;

    // todo add binds

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
