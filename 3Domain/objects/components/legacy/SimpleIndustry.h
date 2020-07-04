//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_SIMPLEINDUSTRY_H
#define BIG3DFLUFFY_SIMPLEINDUSTRY_H

#include "../interfaces/QRIndustry.h"
#include "SimplePoint3D.h"
#include "SimpleEdge3D.h"
#include "SimpleCamera3D.h"

// todo deprecated
/*
class SimplePoint3DCreator: public QRPoint3DCreator {
public:
    explicit SimplePoint3DCreator(QRPointStyle s = QRPointStyle()): st(s) {}
    virtual uptr<QRPoint3D> create(const Vector3D &vec, const Vector3D &bind = Vector3D()) {
        return uptr<QRPoint3D>(new SimplePoint3D(vec, bind, st));
    }
private:
    QRPointStyle st;
};

class SimpleEdge3DCreator: public QREdge3DCreator {
public:
    SimpleEdge3DCreator(QREdgeStyle s = QREdgeStyle()): st(s) {}
    virtual uptr<QREdge3D> create(sptr<QRPoint3D> start, sptr<QRPoint3D> end) {
        return uptr<QREdge3D>(new SimpleEdge3D(start, end, st));
    }
private:
    QREdgeStyle st;
};


// len is the distance from origin to it's bindPoint (strictly by oY)
class SimpleFrontalCamera3DCreator: public QRCamera3DCreator {
public: // todo magic constant 010
    SimpleFrontalCamera3DCreator(float len): viewUp(0, 1, 0), len(len), imp(new OrthogonalProjection) {}
    virtual uptr<QRCamera3D> create(float w, float h,
                                               const Vector3D &origin) {
        return uptr<QRCamera3D>(new SimpleCamera3D(w, h, imp, origin,
                                                              viewUp, Vector3D(origin + Vector3D({0,0,len}))));
    }
private:
    sptr<ProjectionImp> imp;
    Vector3D viewUp;
    float len;
};

class SimpleBasicObject3DFactory: public QRObject3DFactory {
public:
    SimpleBasicObject3DFactory(QRColor pc = QRColor("white"), QRColor ec = QRColor("green")): pc(pc), ec(ec) {}
    virtual uptr<QRPoint3D> createPoint(const Vector3D &vec,
                                                   const Vector3D &bind = Vector3D()) {return point_cr.create(vec,bind);}
    virtual uptr<QREdge3D> createEdge(sptr<QRPoint3D> start,
                                                 sptr<QRPoint3D> end) {return edge_cr.create(start, end);}
    virtual uptr<QRCamera3D> createCamera(float w, float h,
                                                     const Vector3D &origin) {return camera_cr.create(w,h,origin);}
private:
    QRColor pc, ec;
    SimplePoint3DCreator point_cr = SimplePoint3DCreator(pc);
    SimpleEdge3DCreator edge_cr = SimpleEdge3DCreator(ec);
    SimpleFrontalCamera3DCreator camera_cr = SimpleFrontalCamera3DCreator(10);
};
*/

#endif //BIG3DFLUFFY_SIMPLEINDUSTRY_H
