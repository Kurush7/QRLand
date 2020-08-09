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

 camera model: coordinates: Y is up, X is horizontally right (XY is projective plane),
 Z is set FROM viewer, so, the less Z coordinate, the nearer the object is to viewer.
 projective center lies on Z-axis with Z = 0, also x=y=0, so it has (0,0,0) in local coords.
 origin is center's coordinates on the scene.

 viewUpVector - local YAxis, deepVector - local ZAxis, both normalized

 nearCutter, farCutter - z-coordinate of nearest(farest) visible plane
 screen - z-coord of projective screen.
 */

// todo bridge: projection implementation

class Camera3DMemento;
class Camera3D: public QRCamera3D {
public:
    friend class Camera3DMemento;

    Camera3D(float w, float h, const Vector3D &origin, float screen,
             float nearCutter, float farCutter = QRINF);
    ~Camera3D() {p.reset();}
    virtual sptr<QRObject3D> copy() {return sptr<QRCamera3D>(new Camera3D(width, height,
            origin, screen, nearCutter, farCutter));}
    virtual uptr<QRMemento> save();
    virtual void acceptVisitor(const sptr<QRVisitor>& visitor) {visitor->visitCamera3D(p);}
    wptr<QRCamera3D> getPointer() {return p;}


    virtual bool isVisible(){return false;}

    virtual void move(const Vector3D &move);
    virtual void scale(float sx, float sy);
    virtual void scale(float scale);
    virtual void rotate(const Vector3D &rotate);

    virtual sptr<QRTransformer3D> getAxisTransformer() const {return axisTransformer;}
    virtual sptr<QRTransformer3D> getProjectionTransformer() const {return projector;}
    virtual int isVisibleSphere(const Vector3D &center, float rad);

    // todo normal must already be transformed!!!!!!
    virtual bool isFrontFace(const Vector3D &normal);
    // todo polygon already projected!!!!
    virtual Vector3D getScreen() const {return {0,0,width, height};}

    virtual const Vector3D& getOrigin() const {return origin;}
    virtual void setOrigin(const Vector3D &p) { origin = p; defineAxisTransformer();}

    virtual float getWidth() const {return width;}
    virtual void setWidth(float w) {width = w; defineFrustrum();}
    virtual float getHeight() const {return height;}
    virtual void setHeight(float h) {h = height; defineFrustrum();}

    virtual const Vector3D& getBind() const {return bind;}
    virtual void setBind(const Vector3D &b) {bind = b;}

    // todo not all params considered
    virtual bool operator==(const QRCamera3D &b) const {return origin == b.getOrigin() &&
                                                               width == b.getWidth() &&
                                                               height == b.getHeight();}
private:
    sptr<Camera3D> p;
    Vector3D origin, rotated_origin, viewUpVector, deepVector, bind;
    float nearCutter, farCutter, screen;
    Vector3D frustrum[6];

    sptr<QRTransformer3D> axisTransformer, projector;

    void defineAxisTransformer();
    void defineProjectionTransformer();
    void defineFrustrum();

};


#endif //BIG3DFLUFFY_CAMERA3D_H
