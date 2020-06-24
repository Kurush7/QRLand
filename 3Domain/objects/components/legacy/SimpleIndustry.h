//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_SIMPLEINDUSTRY_H
#define BIG3DFLUFFY_SIMPLEINDUSTRY_H

#include "../interfaces/QRIndustry.h"
#include "SimplePoint3D.h"
#include "SimpleEdge3D.h"
#include "SimpleCamera3D.h"


class QRPoint3DCreator: public BaseQRPoint3DCreator {
public:
    explicit QRPoint3DCreator(QRPointStyle s = QRPointStyle()):st(s) {}
    virtual uptr<QRPoint3D> create(const Vector3D &vec, const Vector3D &bind = Vector3D()) {
        return uptr<QRPoint3D>(new SimplePoint3D(vec, bind, st));
    }
private:
    QRPointStyle st;
};

class Edge3DCreator: public BaseEdge3DCreator {
public:
    Edge3DCreator(QREdgeStyle s = QREdgeStyle()):st(s) {}
    virtual uptr<QREdge3D> create(sptr<QRPoint3D> start, sptr<QRPoint3D> end) {
        return uptr<QREdge3D>(new SimpleEdge3D(start, end, st));
    }
private:
    QREdgeStyle st;
};


// len is the distance from origin to it's bindPoint (strictly by oY)
class FrontalCamera3DCreator: public BaseCamera3DCreator {
public: // todo magic constant 010
    FrontalCamera3DCreator(double len): viewUp(0,1,0), len(len), imp(new OrthogonalProjection) {}
    virtual uptr<QRCamera3D> create(double w, double h,
                                               const Vector3D &origin) {
        return uptr<QRCamera3D>(new SimpleCamera3D(w, h, imp, origin,
                                                              viewUp, Vector3D(origin + Vector3D({0,0,len}))));
    }
private:
    sptr<ProjectionImp> imp;
    Vector3D viewUp;
    double len;
};

class BasicObject3DFactory: public AbstractObject3DFactory {
public:
    BasicObject3DFactory(QRColor pc = QRColor("white"), QRColor ec = QRColor("green")): pc(pc), ec(ec) {}
    virtual uptr<QRPoint3D> createPoint(const Vector3D &vec,
                                                   const Vector3D &bind = Vector3D()) {return point_cr.create(vec,bind);}
    virtual uptr<QREdge3D> createEdge(sptr<QRPoint3D> start,
                                                 sptr<QRPoint3D> end) {return edge_cr.create(start, end);}
    virtual uptr<QRCamera3D> createCamera(double w, double h,
                                                     const Vector3D &origin) {return camera_cr.create(w,h,origin);}
private:
    QRColor pc, ec;
    QRPoint3DCreator point_cr = QRPoint3DCreator(pc);
    Edge3DCreator edge_cr = Edge3DCreator(ec);
    FrontalCamera3DCreator camera_cr = FrontalCamera3DCreator(10);
};


#endif //BIG3DFLUFFY_SIMPLEINDUSTRY_H
