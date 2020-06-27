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

#include "textures/QRTexture.h"
#include "exceptions/QRObjectException.h"
#include "mementos/QRMemento.h"
#include "visitors/QRVisitorInterface.h"

// todo prototypes for copying
// todo compares

//  TODO GLOBAL: CONST REFERENCES IN FUNCTIONS INSTEAD OF VALUES FOR PTRS


class QRObject;
using ObjectIterator = QRVectorIterator<sptr<QRObject>>;

class QRObject {
public:
    virtual uptr<QRMemento> save() = 0;
    virtual void acceptVisitor(const sptr<QRVisitor>& visitor) = 0;

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

#endif //KG_BASEOBJECT_H
