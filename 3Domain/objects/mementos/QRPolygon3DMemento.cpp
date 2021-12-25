//
// Created by kurush on 27.06.2020.
//

#include "QRPolygon3DMemento.h"

QRPolygon3DMemento::QRPolygon3DMemento(sptr<QRPolygon3D> p) {
    object = p;
    points = p->getPoints();
    plane = p->getPlane();
    texture = p->getTexture();
}

void QRPolygon3DMemento::restore() {
    if (object.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointerException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to create memento!");
    }

    sptr<QRPolygon3D> p(object);
    p->setPoints(points.begin());
    p->setTexture(texture);
}