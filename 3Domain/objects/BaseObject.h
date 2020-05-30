//
// Created by kurush on 30.05.2020.
//

#ifndef KG_BASEOBJECT_H
#define KG_BASEOBJECT_H

#include <memory>
#include "../math/QRMath.h"
#include "exceptions/QRObjectException.h"

class Memento {
public:
    virtual void restore() = 0;
};

class BaseObject {
public:
    virtual std::unique_ptr<Memento> save() = 0;
    // todo accept visitor
    virtual bool isVisible() {return visible;}
    virtual bool isComposite() = 0;
protected:
    bool visible = true;
};

class Object3D: public BaseObject {
public:
    bool isComposite() {return false;}
};


enum QRColor {
    white, black, blue, red, yellow, green
};
class Style {
public:
    Style() {}
    Style(QRColor c): color(c) {}
    QRColor color = white;

    virtual bool operator==(const Style &s) const{return color == s.color;}
};
class PointStyle: public Style {
public:
    PointStyle(){}
    PointStyle(QRColor c): Style(c) {}

};
class EdgeStyle: public Style {
public:
    EdgeStyle(){}
    EdgeStyle(QRColor c): Style(c) {}
};


#endif //KG_BASEOBJECT_H
