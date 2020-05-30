//
// Created by kurush on 30.05.2020.
//

#ifndef KG_INDUSTRY_H
#define KG_INDUSTRY_H

#include "Point3D.h"
#include "Edge3D.h"

class BasePoint3DCreator {
public:
    virtual std::unique_ptr<BasePoint3D> create() = 0;
};

class Point3DCreator: public BasePoint3DCreator {
public:
    Point3DCreator(const Vector3D &vec, const Vector3D &bind = Vector3D(), PointStyle s = PointStyle()):
        vec(vec), bind(bind), st(s) {}
    virtual std::unique_ptr<BasePoint3D> create() {
        return std::unique_ptr<BasePoint3D>(new Point3D(vec, bind, st));
    }
private:
    Vector3D vec, bind;
    PointStyle st;
};


class BaseEdge3DCreator {
public:
    virtual std::unique_ptr<BaseEdge3D> create() = 0;
};

class Edge3DCreator: public BaseEdge3DCreator {
public:
    Edge3DCreator(std::weak_ptr<BasePoint3D> start, std::weak_ptr<BasePoint3D> end, EdgeStyle s = EdgeStyle()):
            start(start), end(end), st(s) {}
    virtual std::unique_ptr<BaseEdge3D> create() {
        return std::unique_ptr<BaseEdge3D>(new Edge3D(start, end, st));
    }
private:
    std::weak_ptr<BasePoint3D> start, end;
    EdgeStyle st;
};

#endif //KG_INDUSTRY_H
