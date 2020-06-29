//
// Created by kurush on 26.06.2020.
//

#ifndef BIG3DFLUFFY_POINT3D_H
#define BIG3DFLUFFY_POINT3D_H

#include "../interfaces/QRPoint3D.h"

/**
 bind operations not supported for this instance
 Point keeps vector without considering 4th coordinate (projection one). one must be accurate working with
 Vector3D representation of a point, for it uses 4th coord same as others!
 when setting a vector, 4th coordinate is simply zeroed (todo normalize instead?)

 Relative methods are legacy from times when points keps their bind. todo not needed any more?
 */
class Point3D: public QRPoint3D {
public:
    Point3D() {}
    Point3D(const Vector3D &vec, QRPointStyle s = QRPointStyle());
    Point3D(const double x, const double y, const double z, QRPointStyle s = QRPointStyle());

    virtual const Vector3D getVector() const {return vec;}
    virtual const Vector3D getRelativeVector() const {return getVector();}
    virtual void setVector(const Vector3D &v) {vec = v; vec[3] = 0;}
    virtual void setRelativeVector(const Vector3D &v) {setVector(v);}

    virtual const Vector3D& getBind() const {return vec;}
    virtual void setBind(const Vector3D &b) {}

private:
    Vector3D vec;
};


#endif //BIG3DFLUFFY_POINT3D_H
