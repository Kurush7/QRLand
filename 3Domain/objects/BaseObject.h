//
// Created by kurush on 30.05.2020.
//

#ifndef KG_BASEOBJECT_H
#define KG_BASEOBJECT_H

#include <memory>
#include "../math/QRMath.h"
#include "../containers/QRContainers.h"
#include "exceptions/QRObjectException.h"

class Memento {
public:
    virtual void restore() = 0;
};

class BaseObject;
using ObjectIterator = QRVectorIterator<std::shared_ptr<BaseObject>>;

class BaseObject {
public:
    virtual std::unique_ptr<Memento> save() = 0;
    // todo accept visitor
    virtual bool isVisible() {return visible;}
    virtual bool isComposite() = 0;

    virtual ObjectIterator begin() = 0;
    virtual ObjectIterator end() = 0;

protected:
    bool visible = true;
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
