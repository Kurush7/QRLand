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

#endif //KG_INDUSTRY_H
