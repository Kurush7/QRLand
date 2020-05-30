//
// Created by kurush on 30.05.2020.
//

#ifndef KG_BASEOBJECT_H
#define KG_BASEOBJECT_H

#include <memory>
#include "../math/QRMath.h"

class Memento {
public:
    virtual void restore() = 0;
};

class BaseObject {
public:
    virtual std::unique_ptr<Memento> save() = 0;
    // todo accept visitor
    virtual bool isVisible() = 0;
    virtual bool isComposite() = 0;
protected:
    bool visible;
};

class Object3D: public BaseObject {
public:
    bool isComposite() {return false;}
    virtual const Vector3D& getBind() const = 0;
    virtual void setBind(const Vector3D&) = 0;
};


enum QRColor {
    white, black, blue, red, yellow, green
};
struct Style {
    QRColor color;
};
struct PointStyle: public Style {};
struct EdgeStyle: public Style {};


#endif //KG_BASEOBJECT_H
