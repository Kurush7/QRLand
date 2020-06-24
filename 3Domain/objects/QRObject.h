//
// Created by kurush on 30.05.2020.
//

#ifndef KG_BASEOBJECT_H
#define KG_BASEOBJECT_H

#include <memory>
#include <vector>

#include "../QRDefines.h"

#include "../math/QRMath.h"
#include "../containers/QRContainers.h"
#include "exceptions/QRObjectException.h"
#include "mementos/QRMemento.h"

// todo prototypes for copying
// todo compares

class QRPoint3D;
class QREdge3D;
class QRCamera3D;
class BaseFrame3D;

// todo move to visitor directory???
class QRVisitor {
public:
    QRVisitor() { p = sptr<QRVisitor>(this, [](void *ptr){});}
    virtual void visitPoint3D(sptr<QRPoint3D> point) = 0;
    virtual void visitEdge3D(sptr<QREdge3D> edge) = 0;
    virtual void visitCamera3D(sptr<QRCamera3D> camera) = 0;
    virtual void visitFrame3D(sptr<BaseFrame3D> frame);

protected:
    sptr<QRVisitor> p;
};


class QRObject;
using ObjectIterator = QRVectorIterator<sptr<QRObject>>;

class QRObject {
public:
    virtual uptr<QRMemento> save() = 0;
    virtual void acceptVisitor(sptr<QRVisitor> visitor) = 0;

    virtual bool isVisible() {return visible;}
    virtual bool isComposite() = 0;
    virtual bool isSelected() {return selected;}
    // todo is_editable

    virtual void setSelected(bool x) {selected = x;}

    virtual ObjectIterator begin() = 0;
    virtual ObjectIterator end() = 0;

protected:
    bool visible = true;
    bool selected = false;
};


class QRObject3D: public QRObject {
public:
    virtual bool isComposite() {return false;}
    virtual ObjectIterator begin() {return ObjectIterator();}
    virtual ObjectIterator end()  {return ObjectIterator();}
};

// todo move to textures directory/file
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
