//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_SIMPLEPOINT3D_H
#define BIG3DFLUFFY_SIMPLEPOINT3D_H

#include "../interfaces/QRPoint3D.h"

class SimplePoint3D: public QRPoint3D {
public:
    SimplePoint3D() { vec -= bindPoint;}
    SimplePoint3D(const Vector3D &vec, const Vector3D &bind = Vector3D(), QRPointStyle s = QRPointStyle());
    SimplePoint3D(const double x, const double y, const double z, const Vector3D &bind = Vector3D(), QRPointStyle s = QRPointStyle());

    virtual const Vector3D getVector() const {return vec + bindPoint;}
    virtual const Vector3D getRelativeVector() const {return vec;}
    virtual void setVector(const Vector3D &v) {vec = v - bindPoint;}
    virtual void setRelativeVector(const Vector3D &v) {vec = v;}
    virtual const Vector3D& getBind() const {return bindPoint;}
    virtual void setBind(const Vector3D &b) {vec += bindPoint - b; bindPoint = b;}

private:
    Vector3D vec;
    Vector3D bindPoint;
};

#endif //BIG3DFLUFFY_SIMPLEPOINT3D_H
