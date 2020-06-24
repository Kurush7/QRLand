//
// Created by kurush on 24.06.2020.
//

#include "QRPoint3DMemento.h"

QRPoint3DMemento::QRPoint3DMemento(sptr<QRPoint3D> p) {
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
    sptr<QRPoint3D> p(object);
    p->setVector(point);
    p->setBind(bind);
    p->setStyle(style);
}