//
// Created by kurush on 30.05.2020.
//

#ifndef KG_BASEOBJECT_H
#define KG_BASEOBJECT_H

#include <memory>
#include "../math/QRMath.h"
#include "../containers/QRContainers.h"
#include "exceptions/QRObjectException.h"

class BaseQRPoint3D;
class BaseEdge3D;
class BaseCamera3D;
class BaseFrame3D;
class Visitor {
public:
    Visitor() {p = std::shared_ptr<Visitor>(this, [](void *ptr){});}
    virtual void visitPoint3D(std::shared_ptr<BaseQRPoint3D> point) = 0;
    virtual void visitEdge3D(std::shared_ptr<BaseEdge3D> edge) = 0;
    virtual void visitCamera3D(std::shared_ptr<BaseCamera3D> camera) = 0;
    virtual void visitFrame3D(std::shared_ptr<BaseFrame3D> frame);

protected:
    std::shared_ptr<Visitor> p;
};

class Memento {
public:
    virtual void restore() = 0;
};

class BaseObject;
using ObjectIterator = QRVectorIterator<std::shared_ptr<BaseObject>>;

class BaseObject {
public:
    virtual std::unique_ptr<Memento> save() = 0;
    virtual void acceptVisitor(std::shared_ptr<Visitor> visitor) = 0;
    virtual bool isVisible() {return visible;}
    virtual bool isComposite() = 0;
    virtual bool isSelected() {return selected;}
    virtual void setSelected(bool x) {selected = x;}

    virtual ObjectIterator begin() = 0;
    virtual ObjectIterator end() = 0;

protected:
    bool visible = true;
    bool selected = false;
};


/*class ComponentIterator: public std::iterator<std::output_iterator_tag, BaseObject> {
public:
    ComponentIterator(std::shared_ptr<BaseObject> obj, bool is_end=false): obj(obj), is_end(is_end) {}

    virtual bool operator!=(ComponentIterator const &it) const {return is_end != it.is_end;}
    virtual bool operator==(ComponentIterator const &it) const {return is_end == it.is_end;}

    virtual const BaseObject& operator*() const {
        if (obj.expired()) {
            time_t t = time(nullptr);
            throw QRBadPointerException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to get edge's point!");
        }
        return *obj.lock();}
    virtual const BaseObject* operator->() const {
        if (obj.expired()) {
            time_t t = time(nullptr);
            throw QRBadPointerException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to get edge's point!");
        }
        return obj.lock().get();
    }

    virtual explicit operator bool() {return is_end;}

    virtual ComponentIterator& operator++() {is_end = true; return *this;}
    virtual ComponentIterator operator++(int) {ComponentIterator x = *this; is_end = true; return x;}
private:
    bool is_end = false;
    std::weak_ptr<BaseObject> obj;
};*/


class Object3D: public BaseObject {
public:
    virtual bool isComposite() {return false;}
    virtual ObjectIterator begin() {return ObjectIterator();}
    virtual ObjectIterator end()  {return ObjectIterator();}
};


class QRColor {
public:
    // todo accurate color string mapping
    QRColor() {}
    QRColor (std::string name) {
        if (name == "white") r = b = g = 255;
        if (name == "black") r=b=g=0;
        if (name == "green") g = 255;
        if (name == "blue") b = 255;
        if (name == "red") r = 255;
    }
    QRColor(int r, int g, int b): r(r), g(g), b(b) {}
    bool operator==(const QRColor &c) const{return r==c.r && b==c.b && g==c.g;}
    int r=0, g=0, b=0;
};
class Style {
public:
    Style() {}
    Style(QRColor c): color(c) {}
    QRColor color = QRColor("white");

    virtual bool operator==(const Style &s) const{return color == s.color;}
};
class QRPointStyle: public Style {
public:
    QRPointStyle(){}
    QRPointStyle(QRColor c): Style(c) {}

};
class QREdgeStyle: public Style {
public:
    QREdgeStyle(){}
    QREdgeStyle(QRColor c): Style(c) {}
};

class ColorKeeper {
public:
    ColorKeeper(QRPointStyle p, QREdgeStyle e): pointStyle(p), edgeStyle(e) {}
    QRPointStyle pointStyle;
    QREdgeStyle edgeStyle;
};

#endif //KG_BASEOBJECT_H
