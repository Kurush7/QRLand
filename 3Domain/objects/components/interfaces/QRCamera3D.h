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
 is necessary when forming projection matrix

 getScreen() accepts a CONVEX polygon (not checked, so be accurate),
 projects it into scene plane, then cuts it with screen borders (rectangle) and returns
 resulting CONVEX polygon (z-coords TODO)
 */
class QRCamera3D: public QRObject3D {
public:
    QRCamera3D(double w, double h): width(w), height(h) {
        p = sptr<QRCamera3D>(this, [](void *ptr){});}
    ~QRCamera3D() {p.reset();}

    virtual void acceptVisitor(const sptr<QRVisitor>& visitor) {visitor->visitCamera3D(p);}
    virtual uptr<QRMemento> save();

    virtual sptr<QRTransformer3D> getAxisTransformer() const = 0;
    virtual sptr<QRTransformer3D> getProjectionTransformer() const = 0;
    virtual bool isVisibleSphere(const Vector3D &center, double rad) = 0;
    virtual bool isFrontFace(const Vector3D &normal) = 0;

    // v[0], v[1] - x,y of center, v[2] - width, v[3] = height
    virtual Vector3D getScreen() const = 0;


    virtual void move(const Vector3D &move) = 0;
    virtual void scale(double sx, double sy) = 0;
    virtual void scale(double scale) = 0;
    virtual void rotate(const Vector3D &rotate) = 0;

    virtual const Vector3D& getBind() const = 0;
    virtual void setBind(const Vector3D &b) = 0;

    // todo isVisibleSphere
    // todo to camera coords?

    // todo
    //virtual bool operator==(const QRCamera3D &b) const = 0;
    //virtual QRCamera3D& operator=(const QRCamera3D &p) {}

// getters & setters
    virtual double getWidth() const = 0;
    virtual void setWidth(double w) = 0;
    virtual double getHeight() const = 0;
    virtual void setHeight(double h) = 0;

    virtual const Vector3D& getOrigin() const = 0;
    virtual void setOrigin(const Vector3D&) = 0;

    // todo add binds

    wptr<QRCamera3D> getPointer() {return p;}

private:
    sptr<QRCamera3D> p;
protected:
    double width, height;
};

#endif //BIG3DFLUFFY_QRCAMERA3D_H
