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

class Camera3D: public QRCamera3D {
public:
    Camera3D(double w, double h, const sptr<ProjectionImp> &p, const Vector3D &origin, double screen,
             double nearCutter, double farCutter = QRINF);

    virtual bool isVisible(){return false;}

    virtual void move(const Vector3D &move);
    virtual void scale(double sx, double sy);
    virtual void scale(double scale);
    virtual void rotate(const Vector3D &rotate);

    virtual uptr<QRTransformer3D> getProjectionTransformer();

    virtual const Vector3D& getOrigin() const {return origin;}
    virtual void setOrigin(const Vector3D &p) { origin = p;}

    // todo not all params considered
    virtual bool operator==(const QRCamera3D &b) const {return origin == b.getOrigin() &&
                                                               width == b.getWidth() &&
                                                               height == b.getHeight();}
private:
    Vector3D origin, viewUpVector, deepVector;
    double nearCutter, farCutter, screen;
};


#endif //BIG3DFLUFFY_CAMERA3D_H
