//
// Created by kurush on 30.05.2020.
//

#include "Point3D.h"

using namespace std;


unique_ptr<Memento> BasePoint3D::save() {
    return unique_ptr<Memento>(new Point3DMemento(p));
}

Point3DMemento::Point3DMemento(weak_ptr<BasePoint3D> wp) {
    if (wp.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointerException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to create memento!");
    }

    object = wp;
    shared_ptr<BasePoint3D> p(wp);
    point = p->getVector();
    bind = p->getBind();
    style = p->getStyle();
}

void Point3DMemento::restore() {
    if (object.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointerException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to create memento!");
    }
    shared_ptr<BasePoint3D> p(object);
    p->setVector(point);
    p->setBind(bind);
    p->setStyle(style);
}

Point3D::Point3D(const Vector3D &_vec, const Vector3D &bind, PointStyle s): BasePoint3D(s),
vec(_vec), bindPoint(bind) {
    vec -= bind;
}

Point3D::Point3D(const double x, const double y, const double z, const Vector3D &bind, PointStyle s)
: BasePoint3D(s), vec(x, y, z), bindPoint(bind) {
    vec -= bind;
}