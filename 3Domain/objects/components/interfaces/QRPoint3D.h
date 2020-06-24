//
// Created by kurush on 30.05.2020.
//

#ifndef QR_POINT3D_H
#define QR_POINT3D_H

#include "objects/QRObject.h"

class QRPoint3DMemento;

class QRPoint3D: public QRObject3D {
public:
    QRPoint3D(QRPointStyle s = QRPointStyle()): style(s) {p = sptr<QRPoint3D>(this, [](void *ptr){});}
    ~QRPoint3D() {p.reset();}

    virtual uptr<QRMemento> save();
    virtual void acceptVisitor(sptr<QRVisitor> visitor) {visitor->visitPoint3D(p);}
    sptr<QRPoint3D> getPointer() {return p;}

    virtual const Vector3D getPoint() const = 0;
    virtual const Vector3D getRelativePoint() const = 0;

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
