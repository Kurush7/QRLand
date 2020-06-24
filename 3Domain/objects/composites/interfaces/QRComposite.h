//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_QRCOMPOSITE_H
#define BIG3DFLUFFY_QRCOMPOSITE_H


#include "objects/QRObject.h"

class GroupMemento;
class QRComposite: public QRObject {
public:
    explicit QRComposite(QRVector<sptr<QRObject>> &obj): objects(obj) {
        p = sptr<QRComposite>(this, [](void *ptr){});
    }
    virtual bool isComposite() {return true;}
    virtual void setSelected(bool x) {
        for (auto obj: objects) obj->setSelected(x);
        selected = x;
    }

    // todo iterators only!
    virtual const QRVector<sptr<QRObject>>& getObjects() {return objects;}
    virtual void setObjects(QRVector<sptr<QRObject>>&obj) { objects = obj;}

    virtual uptr<QRMemento> save();

private:
    sptr<QRComposite> p;
protected:
    QRVector<sptr<QRObject>> objects;
};



#endif //BIG3DFLUFFY_QRCOMPOSITE_H
