//
// Created by kurush on 30.05.2020.
//

#ifndef KG_POINT3D_H
#define KG_POINT3D_H

#include "BaseObject.h"

class Point3DMemento;
class BasePoint3D: public Object3D {
public:
    BasePoint3D(PointStyle s = PointStyle()): style(s) {
        p = std::shared_ptr<BasePoint3D>(this, [](void *ptr){});
    }
    ~BasePoint3D() {p.reset();}

    virtual std::unique_ptr<Memento> save();

    virtual const Vector3D& getVector() const = 0;
    virtual void setVector(const Vector3D&) = 0;
    virtual const PointStyle& getStyle() const {return style;}
    virtual void setStyle(const PointStyle &s) {style = s;}

    virtual const Vector3D& getBind() const = 0;
    virtual void setBind(const Vector3D&) = 0;

    std::weak_ptr<BasePoint3D> getPointer() {return p;}

    virtual bool operator==(const BasePoint3D &b) const {return getVector() == b.getVector() &&
                                                          getBind() == b.getBind() &&
                                                          getStyle() == b.getStyle();}
    virtual BasePoint3D& operator=(const BasePoint3D &p) {style = p.style;}
private:
    PointStyle style;
    std::shared_ptr<BasePoint3D> p;
};

class Point3DMemento: public Memento {
public:
    Point3DMemento(std::weak_ptr<BasePoint3D>);
    virtual void restore();

private:
    std::weak_ptr<BasePoint3D> object;
    Vector3D point, bind;
    PointStyle style;
};


class Point3D: public BasePoint3D {
public:
    Point3D() {}
    Point3D(const Vector3D &vec, const Vector3D &bind = Vector3D(), PointStyle s = PointStyle());
    Point3D(const double x, const double y, const double z, const Vector3D &bind = Vector3D(), PointStyle s = PointStyle());

    virtual const Vector3D& getVector() const {return vec;}
    virtual void setVector(const Vector3D &v) {vec = v;}
    virtual const Vector3D& getBind() const {return bindPoint;}
    virtual void setBind(const Vector3D &b) {bindPoint = b;}

private:
    Vector3D vec;
    Vector3D bindPoint;
};


#endif //KG_POINT3D_H
