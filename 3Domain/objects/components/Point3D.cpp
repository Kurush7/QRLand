//
// Created by kurush on 30.05.2020.
//

#include "Point3D.h"

using namespace std;


unique_ptr<Memento> BaseQRPoint3D::save() {
    return unique_ptr<Memento>(new QRPoint3DMemento(p));
}

QRPoint3DMemento::QRPoint3DMemento(shared_ptr<BaseQRPoint3D> p) {
    object = p;
    point = p->getPoint();
    bind = p->getBind();
    style = p->getStyle();
}

void QRPoint3DMemento::restore() {
    if (object.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointerException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to create memento!");
    }
    shared_ptr<BaseQRPoint3D> p(object);
    p->setVector(point);
    p->setBind(bind);
    p->setStyle(style);
}

QRPoint3D::QRPoint3D(const Vector3D &_vec, const Vector3D &bind, QRPointStyle s): BaseQRPoint3D(s),
vec(_vec), bindPoint(bind) {
    vec -= bind;
}

QRPoint3D::QRPoint3D(const double x, const double y, const double z, const Vector3D &bind, QRPointStyle s)
: BaseQRPoint3D(s), vec(x, y, z), bindPoint(bind) {
    vec -= bind;
}