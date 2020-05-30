//
// Created by kurush on 30.05.2020.
//

#ifndef KG_INDUSTRY_H
#define KG_INDUSTRY_H

#include "Point3D.h"
#include "Edge3D.h"

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

#endif //KG_INDUSTRY_H
