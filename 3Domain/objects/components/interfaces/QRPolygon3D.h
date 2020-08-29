//
// Created by kurush on 26.06.2020.
//

#ifndef BIG3DFLUFFY_QRPOLYGON3D_H
#define BIG3DFLUFFY_QRPOLYGON3D_H

#include "QRPoint3D.h"
#include "../../../textures/QRTexture.h"

/**
    getPlane(): abcd of a plane
    getNormal(): xyz of a normal vector (one of)
    where(const Vector3D&): -1, 1 or 0: position of point as to the plane
    switchNormal() - set opposite normal. Constructor defines a normal randomly
    getSize() - number of points in a polygon
 */

enum PolyPosition {
    BEHIND = -1,
    FRONT = 1,
    ON = 0
};

using PolygonIterator = QRVectorIterator<sptr<QRPolygon3D>>;
using IndexIterator = QRVectorIterator<int32_t>;

class QRPolygon3D: public QRObject3D {
public:
    QRPolygon3D() {p = sptr<QRPolygon3D>(this, [](void *ptr){});}
    ~QRPolygon3D() {p.reset();}

    virtual void acceptVisitor(const sptr<QRVisitor>& visitor) {visitor->visitPolygon3D(p);}
    virtual uptr<QRMemento> save();

    virtual bool isIndexData() const {return false;}
    virtual bool isPointData() const {return true;}
    virtual PointIterator getPoints() const {}
    virtual IndexIterator getPointIndexes() const {}
    virtual sptr<QRPoint3D>* getPurePoints() const {}
    virtual int32_t* getPurePointIndexes() const {}
    virtual void setPoints(const PointIterator &it) {}  // todo deprecated, was used for mementos

    virtual const Vector3D getPlane() const = 0;
    virtual const Vector3D getNormal() const = 0;
    virtual void setNormal(const Vector3D&) = 0;   // TODO VERY ACCURATE WITH THIS
    virtual void updateNormal() = 0;
    // points changed, return new normal instead of applying it. 3rd value contains new d-val
    virtual Vector3D computeNewPlane() = 0;
    virtual void switchNormal() = 0;
    virtual PolyPosition where(const Vector3D&) const = 0;

    virtual int getSize() const = 0;

    virtual const sptr<QRTexture>& getTexture() const = 0;
    virtual sptr<QRTexture>& getTextureUnsafe() = 0;
    virtual void setTexture(const sptr<QRTexture>&) = 0;

    // todo virtual QREdge3D& operator=(){}
    // todo get triangles? pointset....
    // todo bare array getter?
private:
    sptr<QRPolygon3D> p;
};

std::ostream& operator<<(std::ostream &os, const sptr<QRPolygon3D> &p);


#endif //BIG3DFLUFFY_QRPOLYGON3D_H
