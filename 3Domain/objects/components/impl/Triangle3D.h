//
// Created by kurush on 02.08.2020.
//

#ifndef BIG3DFLUFFY_TRIANGLE3D_H
#define BIG3DFLUFFY_TRIANGLE3D_H

#include "../interfaces/QRPolygon3D.h"
#include "math/QRMath.h"

class Triangle3D: public QRPolygon3D {
public:
    Triangle3D() {}
    Triangle3D(sptr<QRPoint3D> p1, sptr<QRPoint3D> p2, sptr<QRPoint3D> p3,
              const sptr<QRTexture> t);
    Triangle3D(initializer_list<sptr<QRPoint3D>> lst,
              const sptr<QRTexture> t);
    ~Triangle3D() {delete arr;}

    virtual sptr<QRObject3D> copy() {return sptr<Triangle3D>(new Triangle3D(p1,p2,p3,texture));}

    virtual PointIterator getPoints() const {return PointIterator ({p1, p2, p3});}
    virtual sptr<QRPoint3D>* getPurePoints() const {return arr;}

    virtual void setPoints(const PointIterator &it0) {
        auto it = it0;
        p1 = *it; ++it;
        p2 = *it; ++it;
        p3 = *it;
        definePlane();}

    virtual const Vector3D getPlane() const {auto p = normal; p[3] = d; return p;}
    virtual const Vector3D getNormal() const {return normal;}
    virtual void setNormal(const Vector3D &n) {
        normal = n;
        normal = lenNorm(normal);
        d = -scalar(normal, p1->getVector());
    }
    virtual void updateNormal() {definePlane();}
    virtual Vector3D computeNewPlane() {
        auto n = defineNormal();
        n[3] = -scalar(normal, p1->getVector());
        return n;
    }
    virtual void switchNormal() {normal = -1 * normal; d *= -1;}
    virtual PolyPosition where(const Vector3D &v) const;

    virtual int getSize() const {return 3;}

    virtual const sptr<QRTexture>& getTexture() const {return texture;}
    virtual void setTexture(const sptr<QRTexture> &t) {texture = t;}

    virtual bool operator==(const QRPolygon3D &b) const;

public:
    sptr<QRPoint3D> p1, p2, p3;
    sptr<QRTexture> texture;

    // todo extra memory.... phew
    sptr<QRPoint3D> *arr = new sptr<QRPoint3D>[3];

    Vector3D normal; // normal: len(a,b,c) = 1, d=1;
    float d;  // plane's d: ax+by+c+D=0

    void definePlane();
    Vector3D defineNormal();
};

#endif //BIG3DFLUFFY_TRIANGLE3D_H
