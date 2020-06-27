//
// Created by kurush on 26.06.2020.
//

#ifndef BIG3DFLUFFY_QRPOLYGON3D_H
#define BIG3DFLUFFY_QRPOLYGON3D_H

#include "QRPoint3D.h"
#include "../../textures/QRTexture.h"

/**
    getPlane(): abcd of a plane
    getNormal(): xyz of a normal vector (one of)
    where(const Vector3D&): -1, 1 or 0: position of point as to the plane
    switchNormal() - set opposite normal. Constructor defines a normal randomly
    getSize() - number of points in a polygon
 */
class QRPolygon3D: public QRObject3D {
public:
    QRPolygon3D() {p = sptr<QRPolygon3D>(this, [](void *ptr){});}
    ~QRPolygon3D() {p.reset();}

    virtual void acceptVisitor(const sptr<QRVisitor>& visitor) {visitor->visitPolygon3D(p);}
    virtual uptr<QRMemento> save();

    virtual PointIterator getPoints() const = 0;
    virtual void setPoints(const PointIterator &it) = 0;

    virtual const Vector3D getPlane() const = 0;
    virtual const Vector3D getNormal() const = 0;
    virtual void switchNormal() = 0;
    virtual int where(const Vector3D&) const = 0;

    virtual int getSize() const = 0;

    virtual const sptr<QRTexture>& getTexture() const = 0;
    virtual void setTexture(const sptr<QRTexture>&) = 0;

    virtual bool operator==(const QRPolygon3D &b) const = 0;

    // todo virtual QREdge3D& operator=(){}
    // todo get triangles? pointset....
    // todo bare array getter?

private:
    sptr<QRPolygon3D> p;
};



#endif //BIG3DFLUFFY_QRPOLYGON3D_H
