//
// Created by kurush on 27.06.2020.
//

#ifndef BIG3DFLUFFY_QRPOLYMODEL3D_H
#define BIG3DFLUFFY_QRPOLYMODEL3D_H

#include "../../QRObject.h"
#include "../../components/interfaces/QRPolygon3D.h"

class QRPolyModel3D;
using ModelIterator = QRVectorIterator<sptr<QRPolyModel3D>>;
using ModelIteratorPtr = sptr<ModelIterator>;

class QRPolyModel3D: public QRComposite {
public:
    explicit QRPolyModel3D(): QRComposite() {
        p_model = sptr<QRPolyModel3D>(this, [](void *ptr){});
    }
    ~QRPolyModel3D() {p_model.reset();}

    virtual void acceptVisitor(const sptr<QRVisitor>& visitor) {}   // todo no fitting method in visitor
    sptr<QRPolyModel3D> getPointer() {return p_model;}
    virtual uptr<QRMemento> save();
    virtual bool isConvex() {return convexity;}
    virtual bool isTriangulated() {return triangulated;}
    virtual void setConvexity(bool x) {convexity = x;}
    virtual void setTriangulated(bool x) {triangulated = x;}

    virtual bool isAdditivePolygons() {return false;}
    virtual void updateCamera(const sptr<QRCamera3D> &camera, std::string *info=nullptr) {}
    virtual void addPolygons(QRVector<sptr<QRPolygon3D>> &polygons, std::string *info=nullptr) {}

    virtual PolygonIterator getPolygons() const = 0;
    virtual PointIterator getPoints() const = 0;
    virtual sptr<QRPolygon3D>* getPurePolygons() const = 0;
    virtual sptr<QRPoint3D>* getPurePoints() const = 0;
    virtual size_t getPolygonCnt() const = 0;
    virtual size_t getPointCnt() const = 0;
    virtual void setValues(PointIterator pt, PolygonIterator pg) = 0;   // for memento only! ?todo friend?

    virtual float getRadius() const = 0;   // radius of circumscribed sphere

    virtual bool operator==(const QRPolyModel3D &b) const;
    //virtual QRModel3D& operator=(const QRModel3D &p) = delete;      // todo!!!
protected:
    bool convexity = false;
    bool triangulated = false;
private:
    sptr<QRPolyModel3D> p_model;
};

#endif //BIG3DFLUFFY_QRPOLYMODEL3D_H
