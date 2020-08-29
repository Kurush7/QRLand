//
// Created by kurush on 29.08.2020.
//

#ifndef BIG3DFLUFFY_INDEXPOLYGON3D_H
#define BIG3DFLUFFY_INDEXPOLYGON3D_H

#include "../interfaces/QRPolygon3D.h"
#include "math/QRMath.h"

class IndexPolygon3D: public QRPolygon3D {
public:
    IndexPolygon3D() {}
    IndexPolygon3D(const QRVector<int32_t> &ind,
              const sptr<QRTexture> t,
              const QRVector<sptr<QRPoint3D>> &points);
    IndexPolygon3D(const QRVector<int32_t> &ind,
                   const sptr<QRTexture> t,
                   const Vector3D &plane);
    IndexPolygon3D(initializer_list<int32_t> lst,
              const sptr<QRTexture> t,
                   const QRVector<sptr<QRPoint3D>> &points);

    virtual sptr<QRObject3D> copy() {return sptr<IndexPolygon3D>(new IndexPolygon3D(points,texture,
            getPlane()));}

    virtual bool isIndexData() const {return true;}
    virtual bool isPointData() const {return false;}
    virtual IndexIterator getPointIndexes() const {return points.begin();}
    virtual int32_t* getPurePointIndexes() const {return points.getPureArray();}

    virtual const Vector3D getPlane() const {auto p = normal; p[3] = d; return p;}
    virtual const Vector3D getNormal() const {return normal;}

    // todo stuff not working
    virtual void setNormal(const Vector3D &n) {}
    virtual void updateNormal() {}
    virtual Vector3D computeNewPlane() {}

    virtual void switchNormal() {normal = -1 * normal; d *= -1;}
    virtual PolyPosition where(const Vector3D &v) const;

    virtual int getSize() const {return points.getSize();}

    virtual const sptr<QRTexture>& getTexture() const {return texture;}
    virtual sptr<QRTexture>& getTextureUnsafe() {return texture;}
    virtual void setTexture(const sptr<QRTexture> &t) {texture = t;}

private:
    QRVector<int32_t> points;
    sptr<QRTexture> texture;

    Vector3D normal; // normal: len(a,b,c) = 1, d=1;
    float d;  // plane's d: ax+by+c+D=0

    void definePlane(const QRVector<sptr<QRPoint3D>> &points);
};


#endif //BIG3DFLUFFY_INDEXPOLYGON3D_H
