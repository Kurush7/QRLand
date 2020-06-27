//
// Created by kurush on 26.06.2020.
//

#ifndef BIG3DFLUFFY_POLYGON3D_H
#define BIG3DFLUFFY_POLYGON3D_H

#include "../interfaces/QRPolygon3D.h"
#include "math/QRMath.h"

class Polygon3D: public QRPolygon3D {
public:
    Polygon3D() {}
    Polygon3D(const QRVector<sptr<QRPoint3D>> &p,
            const sptr<QRTexture> t);
    Polygon3D(initializer_list<sptr<QRPoint3D>> lst,
            const sptr<QRTexture> t);

    virtual PointIterator getPoints() const {return points.begin();}
    virtual void setPoints(const PointIterator &it) {points = it; definePlane();}

    virtual const Vector3D getPlane() const {auto p = normal; p[3] = d; return p;}
    virtual const Vector3D getNormal() const {return normal;}
    virtual void switchNormal() {normal = -1 * normal; d *= -1;}
    virtual int where(const Vector3D &v) const;

    virtual int getSize() const {return points.getSize();}

    virtual const sptr<QRTexture>& getTexture() const {return texture;}
    virtual void setTexture(const sptr<QRTexture> &t) {texture = t;}

    virtual bool operator==(const QRPolygon3D &b) const;

private:
    QRVector<sptr<QRPoint3D>> points;
    sptr<QRTexture> texture;

    Vector3D normal; // normal: len(a,b,c) = 1, d=1;
    double d;  // plane's d: ax+by+c+D=0

    void definePlane();
};

#endif //BIG3DFLUFFY_POLYGON3D_H
