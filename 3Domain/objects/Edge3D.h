//
// Created by kurush on 30.05.2020.
//

#ifndef QR_EDGE3D_H
#define QR_EDGE3D_H

#include "Point3D.h"
#include "BaseObject.h"

class Edge3DMemento;
class BaseEdge3D: public Object3D {
public:
    BaseEdge3D(EdgeStyle s = EdgeStyle()): style(s) {
        p = std::shared_ptr<BaseEdge3D>(this, [](void *ptr){});
    }
    ~BaseEdge3D() {p.reset();}

    virtual std::unique_ptr<Memento> save();

    virtual const std::weak_ptr<BasePoint3D> getStart() const = 0;
    virtual void setStart(std::weak_ptr<BasePoint3D>) = 0;

    virtual const std::weak_ptr<BasePoint3D> getEnd() const = 0;
    virtual void setEnd(std::weak_ptr<BasePoint3D>) = 0;

    virtual const EdgeStyle& getStyle() const {return style;}
    virtual void setStyle(const EdgeStyle &s) {style = s;}

    virtual bool operator==(const BaseEdge3D &b) const {return getStart().lock() == b.getStart().lock() &&
                                                               getEnd().lock() == b.getEnd().lock() &&
                                                               getStyle() == b.getStyle();}
    virtual BaseEdge3D& operator=(const BaseEdge3D &p) {style = p.style;}
private:
    EdgeStyle style;
    std::shared_ptr<BaseEdge3D> p;
};

class Edge3DMemento: public Memento {
public:
    Edge3DMemento(std::weak_ptr<BaseEdge3D>);
    virtual void restore();

private:
    std::weak_ptr<BaseEdge3D> object;
    std::weak_ptr<BasePoint3D> start, end;
    EdgeStyle style;
};


class Edge3D: public BaseEdge3D {
public:
    Edge3D(std::weak_ptr<BasePoint3D> start, std::weak_ptr<BasePoint3D> end, EdgeStyle s = EdgeStyle());
    Edge3D(BasePoint3D &start, BasePoint3D &end, EdgeStyle s = EdgeStyle());

    virtual const std::weak_ptr<BasePoint3D> getStart() const {return start;}
    virtual void setStart(std::weak_ptr<BasePoint3D> p) {start = p;}

    virtual const std::weak_ptr<BasePoint3D> getEnd() const {return end;}
    virtual void setEnd(std::weak_ptr<BasePoint3D> p) {end = p;}

private:
    std::weak_ptr<BasePoint3D> start, end;
};


#endif //QR_EDGE3D_H
