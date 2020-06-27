//
// Created by kurush on 26.06.2020.
//

#ifndef BIG3DFLUFFY_POINT3D_H
#define BIG3DFLUFFY_POINT3D_H

#include "../interfaces/QRPoint3D.h"

// bind operations not supported for this instance

class Point3D: public QRPoint3D {
public:
    Point3D() {}
    Point3D(const Vector3D &vec, QRPointStyle s = QRPointStyle());
    Point3D(const double x, const double y, const double z, QRPointStyle s = QRPointStyle());

    virtual const Vector3D getPoint() const {return vec;}
    virtual const Vector3D getRelativePoint() const {return getPoint();}
    virtual void setVector(const Vector3D &v) {vec = v;}
    virtual void setRelativeVector(const Vector3D &v) {setVector(v);}

    virtual const Vector3D& getBind() const {return vec;}
    virtual void setBind(const Vector3D &b) {}

private:
    Vector3D vec;
};


#endif //BIG3DFLUFFY_POINT3D_H
