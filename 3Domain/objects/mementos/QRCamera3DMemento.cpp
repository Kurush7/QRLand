//
// Created by kurush on 24.06.2020.
//

#include "QRCamera3DMemento.h"

Camera3DMemento::Camera3DMemento(shared_ptr<QRCamera3D> p) {
    object = p;
    origin = p->getOrigin();
    width = p->getWidth();
    height = p->getHeight();
    projector = p->getProjector();
}

void Camera3DMemento::restore() {
    if (object.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointerException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to create memento!");
    }
    shared_ptr<QRCamera3D> p(object);
    p->setOrigin(origin);
    p->setWidth(width);
    p->setHeight(height);
    p->setProjector(projector);
}