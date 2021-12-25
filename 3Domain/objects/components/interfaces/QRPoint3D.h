//
// Created by kurush on 30.05.2020.
//

#ifndef QR_POINT3D_H
#define QR_POINT3D_H

#include "objects/QRObject.h"

using PointIterator = QRVectorIterator<sptr<QRPoint3D>>;

/**
 it must be guaranteed that 4th coordinate (projective) in Vector3D-representation equals 0!
 0, because point is not supposed itself to be used for projection management, and some functions
 like vectorLen or normalization use 4th coord equally to others when in Vector3D
 */
class QRPoint3D: public QRObject3D {
public:
    QRPoint3D(QRPointStyle s = QRPointStyle()): style(s) {p = sptr<QRPoint3D>(this, [](void *ptr){});}
    ~QRPoint3D() {p.reset();}

    virtual uptr<QRMemento> save();
    virtual void acceptVisitor(const sptr<QRVisitor>& visitor) {visitor->visitPoint3D(p);}
    sptr<QRPoint3D> getPointer() {return p;}

    virtual const Vector3D getVector() const = 0;
    virtual const Vector3D getRelativeVector() const = 0;   // todo erase

    virtual void setVector(const Vector3D&) = 0;
    virtual void setRelativeVector(const Vector3D&) = 0;

    virtual const QRPointStyle& getStyle() const {return style;}
    virtual void setStyle(const QRPointStyle &s) {style = s;}

    virtual const Vector3D& getBind() const = 0;
    virtual void setBind(const Vector3D&) = 0;

    virtual bool operator==(const QRPoint3D &b) const;
    virtual QRPoint3D& operator=(const QRPoint3D &p) { style = p.style;}
private:
    QRPointStyle style;
    sptr<QRPoint3D> p;
};


#endif //QR_POINT3D_H
