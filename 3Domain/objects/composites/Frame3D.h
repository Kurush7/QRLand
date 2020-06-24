//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_FRAME3D_H
#define BIG3DFLUFFY_FRAME3D_H

#include "BaseComposite.h"
#include "../components/interfaces/QRPoint3D.h"
#include "../components/interfaces/QREdge3D.h"

class BaseFrame3D: public BaseComposite {
public:
    explicit BaseFrame3D(QRVector<sptr<QRObject>> &obj): BaseComposite(obj) {
        p_frame = sptr<BaseFrame3D>(this, [](void *ptr){});
    }
    ~BaseFrame3D() {p_frame.reset();}

    virtual void acceptVisitor(sptr<QRVisitor> visitor) {visitor->visitFrame3D(p_frame);}

    sptr<BaseFrame3D> getPointer() {return p_frame;}
    virtual uptr<QRMemento> save() override;

    virtual bool operator==(const BaseFrame3D &b) const = delete;
    virtual BaseFrame3D& operator=(const BaseFrame3D &p) = delete;
private:
    sptr<BaseFrame3D> p_frame;
};

class Frame3DMemento: public GroupMemento {
public:
    Frame3DMemento(sptr<BaseFrame3D> obj);
    virtual void restore();

private:
    sptr<MementoAccumulator> memes;
};


class Frame3D: public BaseFrame3D {
public:
    explicit Frame3D(QRVector<sptr<QRObject>> obj): BaseFrame3D(obj) {}

    virtual ObjectIterator begin() {return objects.begin();}
    virtual ObjectIterator end() { return objects.end();}
};


#endif //BIG3DFLUFFY_FRAME3D_H
