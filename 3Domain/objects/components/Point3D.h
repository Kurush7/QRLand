//
// Created by kurush on 30.05.2020.
//

#ifndef QR_POINT3D_H
#define QR_POINT3D_H

#include "../BaseObject.h"

class BaseQRPoint3D: public Object3D {
public:
    BaseQRPoint3D(QRPointStyle s = QRPointStyle()): style(s) {
        p = std::shared_ptr<BaseQRPoint3D>(this, [](void *ptr){});
    }
    ~BaseQRPoint3D() {p.reset();}

    virtual std::unique_ptr<Memento> save();
    virtual void acceptVisitor(std::shared_ptr<Visitor> visitor) {visitor->visitPoint3D(p);}
    virtual const Vector3D getPoint() const = 0;
    virtual const Vector3D getRelativePoint() const = 0;
    virtual void setVector(const Vector3D&) = 0;
    virtual void setRelativeVector(const Vector3D&) = 0;
    virtual const QRPointStyle& getStyle() const {return style;}
    virtual void setStyle(const QRPointStyle &s) {style = s;}

    virtual const Vector3D& getBind() const = 0;
    virtual void setBind(const Vector3D&) = 0;

    std::shared_ptr<BaseQRPoint3D> getPointer() {return p;}

    virtual bool operator==(const BaseQRPoint3D &b) const {return getPoint() == b.getPoint() &&
                                                          getBind() == b.getBind() &&
                                                          getStyle() == b.getStyle();}
    virtual BaseQRPoint3D& operator=(const BaseQRPoint3D &p) {style = p.style;}
private:
    QRPointStyle style;
    std::shared_ptr<BaseQRPoint3D> p;
};

class QRPoint3DMemento: public Memento {
public:
    QRPoint3DMemento(std::shared_ptr<BaseQRPoint3D>);
    virtual void restore();

private:
    std::weak_ptr<BaseQRPoint3D> object;
    Vector3D point, bind;
    QRPointStyle style;
};


class QRPoint3D: public BaseQRPoint3D {
public:
    QRPoint3D() {vec -= bindPoint;}
    QRPoint3D(const Vector3D &vec, const Vector3D &bind = Vector3D(), QRPointStyle s = QRPointStyle());
    QRPoint3D(const double x, const double y, const double z, const Vector3D &bind = Vector3D(), QRPointStyle s = QRPointStyle());

    virtual const Vector3D getPoint() const {return vec + bindPoint;}
    virtual const Vector3D getRelativePoint() const {return vec;}
    virtual void setVector(const Vector3D &v) {vec = v - bindPoint;}
    virtual void setRelativeVector(const Vector3D &v) {vec = v;}
    virtual const Vector3D& getBind() const {return bindPoint;}
    virtual void setBind(const Vector3D &b) {vec += bindPoint; bindPoint = b; vec -= bindPoint;}

private:
    Vector3D vec;
    Vector3D bindPoint;
};


#endif //QR_POINT3D_H
