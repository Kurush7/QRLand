//
// Created by kurush on 30.05.2020.
//

#ifndef KG_INDUSTRY_H
#define KG_INDUSTRY_H

#include "Point3D.h"
#include "Edge3D.h"
#include "Camera3D.h"

class BasePoint3DCreator {
public:
    virtual std::unique_ptr<BasePoint3D> create(const Vector3D &vec, const Vector3D &bind = Vector3D()) = 0;
};

class Point3DCreator: public BasePoint3DCreator {
public:
    explicit Point3DCreator(PointStyle s = PointStyle()):st(s) {}
    virtual std::unique_ptr<BasePoint3D> create(const Vector3D &vec, const Vector3D &bind = Vector3D()) {
        return std::unique_ptr<BasePoint3D>(new Point3D(vec, bind, st));
    }
private:
    PointStyle st;
};


class BaseEdge3DCreator {
public:
    virtual std::unique_ptr<BaseEdge3D> create(std::shared_ptr<BasePoint3D> start, std::shared_ptr<BasePoint3D> end) = 0;
};

class Edge3DCreator: public BaseEdge3DCreator {
public:
    Edge3DCreator(EdgeStyle s = EdgeStyle()):st(s) {}
    virtual std::unique_ptr<BaseEdge3D> create(std::shared_ptr<BasePoint3D> start, std::shared_ptr<BasePoint3D> end) {
        return std::unique_ptr<BaseEdge3D>(new Edge3D(start, end, st));
    }
private:
    EdgeStyle st;
};


class BaseCamera3DCreator {
public:
    virtual std::unique_ptr<BaseCamera3D> create(double w, double h,
            const Vector3D &origin) = 0;
};

// len is the distance from origin to it's bindPoint (strictly by oY)
class FrontalCamera3DCreator: public BaseCamera3DCreator {
public: // todo magic constant 010
    FrontalCamera3DCreator(double len): viewUp(0,1,0), len(len), imp(new OrthogonalProjection) {}
    virtual std::unique_ptr<BaseCamera3D> create(double w, double h,
                                                 const Vector3D &origin) {
        return std::unique_ptr<BaseCamera3D>(new Camera3D(w, h, imp, origin,
                viewUp, Vector3D(origin + Vector3D({0,0,len}))));
    }
private:
    std::shared_ptr<ProjectionImp> imp;
    Vector3D viewUp;
    double len;
};


class AbstractObject3DFactory {
public:
    virtual std::unique_ptr<BasePoint3D> createPoint(const Vector3D &vec, const Vector3D &bind = Vector3D()) = 0;
    virtual std::unique_ptr<BaseEdge3D> createEdge(std::shared_ptr<BasePoint3D> start,
                                                   std::shared_ptr<BasePoint3D> end) = 0;
    virtual std::unique_ptr<BaseCamera3D> createCamera(double w, double h,
                                                       const Vector3D &origin) = 0;
};

class BasicObject3DFactory: public AbstractObject3DFactory {
public:
    BasicObject3DFactory(QRColor pc = yellow, QRColor ec = green): pc(pc), ec(ec) {}
    virtual std::unique_ptr<BasePoint3D> createPoint(const Vector3D &vec,
            const Vector3D &bind = Vector3D()) {return point_cr.create(vec,bind);}
    virtual std::unique_ptr<BaseEdge3D> createEdge(std::shared_ptr<BasePoint3D> start,
                                                   std::shared_ptr<BasePoint3D> end) {return edge_cr.create(start,end);}
    virtual std::unique_ptr<BaseCamera3D> createCamera(double w, double h,
                                                       const Vector3D &origin) {return camera_cr.create(w,h,origin);}
private:
    QRColor pc, ec;
    Point3DCreator point_cr = Point3DCreator(pc);
    Edge3DCreator edge_cr = Edge3DCreator(ec);
    FrontalCamera3DCreator camera_cr = FrontalCamera3DCreator(100);
};

#endif //KG_INDUSTRY_H
