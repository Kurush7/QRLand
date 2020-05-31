//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_FRAME3D_H
#define BIG3DFLUFFY_FRAME3D_H

#include "BaseComposite.h"
#include "../components/Point3D.h"
#include "../components/Edge3D.h"

class BaseFrame3D: public BaseComposite {
public:
    explicit BaseFrame3D(QRVector<std::shared_ptr<BaseObject>> &obj): BaseComposite(obj) {}
    ~BaseFrame3D() {p.reset();}

    std::shared_ptr<BaseFrame3D> getPointer() {return p;}

    virtual bool operator==(const BaseFrame3D &b) const = delete;
    virtual BaseFrame3D& operator=(const BaseFrame3D &p) = delete;
private:
    std::shared_ptr<BaseFrame3D> p;
};

class Frame3D: public BaseFrame3D {
public:
    explicit Frame3D(QRVector<std::shared_ptr<BaseObject>> obj): BaseFrame3D(obj) {}

    virtual ObjectIterator begin() {return objects.begin();}
    virtual ObjectIterator end() { return objects.end();}
};


#endif //BIG3DFLUFFY_FRAME3D_H
