//
// Created by kurush on 27.06.2020.
//

#ifndef BIG3DFLUFFY_QRPOLYMODEL3D_H
#define BIG3DFLUFFY_QRPOLYMODEL3D_H

#include "../../QRObject.h"
#include "../../components/interfaces/QRPolygon3D.h"

class QRPolyModel3D;
using ModelIterator = QRVectorIterator<sptr<QRPolyModel3D>>;

class QRPolyModel3D: public QRComposite {
public:
    explicit QRPolyModel3D(): QRComposite() {
        p_model = sptr<QRPolyModel3D>(this, [](void *ptr){});
    }
    ~QRPolyModel3D() {p_model.reset();}

    virtual void acceptVisitor(const sptr<QRVisitor>& visitor) {}   // todo no fitting method in visitor
    sptr<QRPolyModel3D> getPointer() {return p_model;}
    virtual uptr<QRMemento> save(); // todo make memento

    virtual const PolygonIterator& getPolygons() const = 0;
    virtual const PointIterator& getPoints() const = 0;
    virtual void setValues(const PointIterator &pt, const PolygonIterator &pg) = 0;   // for memento only! ?todo friend?

    virtual bool operator==(const QRPolyModel3D &b) const;
    //virtual QRModel3D& operator=(const QRModel3D &p) = delete;      // todo!!!
private:
    sptr<QRPolyModel3D> p_model;
};

#endif //BIG3DFLUFFY_QRPOLYMODEL3D_H
