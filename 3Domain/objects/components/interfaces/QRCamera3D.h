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
    QRCamera3D(float w, float h): width(w), height(h) {}
    ~QRCamera3D() = default;

    virtual void acceptVisitor(const sptr<QRVisitor>& visitor) = 0;
    virtual uptr<QRMemento> save() = 0;

    virtual sptr<QRTransformer3D> getAxisTransformer() const = 0;
    virtual sptr<QRTransformer3D> getProjectionTransformer() const = 0;

    // 1 - fully visible, 0 - fully invisible, -1 - partly
    virtual int isVisibleSphere(const Vector3D &center, float rad) = 0;
    virtual bool isFrontFace(const Vector3D &normal) = 0;

    // v[0], v[1] - x,y of center, v[2] - width, v[3] = height
    virtual Vector3D getScreen() const = 0;
    virtual const QRVector<Vector3D>& getFrustrum() = 0;

    virtual void move(const Vector3D &move) = 0;
    virtual void scale(float sx, float sy) = 0;
    virtual void scale(float scale) = 0;
    virtual void rotate(const Vector3D &rotate) = 0;

    virtual const Vector3D& getBind() const = 0;
    virtual void setBind(const Vector3D &b) = 0;

    virtual Vector3D getWorldOriginCoord() = 0;

    // todo
    //virtual bool operator==(const QRCamera3D &b) const = 0;
    //virtual QRCamera3D& operator=(const QRCamera3D &p) {}

// getters & setters
    virtual float getWidth() const = 0;
    virtual void setWidth(float w) = 0;
    virtual float getHeight() const = 0;
    virtual void setHeight(float h) = 0;

    virtual const Vector3D& getOrigin() const = 0;
    virtual void setOrigin(const Vector3D&) = 0;

    // todo add binds

protected:
    float width, height;
};

#endif //BIG3DFLUFFY_QRCAMERA3D_H
