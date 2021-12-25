//
// Created by kurush on 24.06.2020.
//

#include "QREdge3DMemento.h"

Edge3DMemento::Edge3DMemento(sptr<QREdge3D> p) {
    object = p;
    start = p->getStart();
    end = p->getEnd();
    style = p->getStyle();
}

void Edge3DMemento::restore() {
    if (object.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointerException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to create memento!");
    }
    if (start.expired() || end.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to create memento!");
    }
    sptr<QREdge3D> p(object);
    p->setStart(start.lock());
    p->setEnd(end.lock());
    p->setStyle(style);
}