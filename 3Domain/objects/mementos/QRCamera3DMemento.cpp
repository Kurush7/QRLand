//
// Created by kurush on 24.06.2020.
//

#include "QRCamera3DMemento.h"

Camera3DMemento::Camera3DMemento(sptr<Camera3D> p) {
    object = p;

    width = p->width;
    height = p->height;

    origin = p->origin;
    viewUpVector = p->viewUpVector;
    deepVector = p->deepVector;
    bind = p->bind;

    nearCutter = p->nearCutter;
    farCutter = p->farCutter;
    screen = p->screen;

}

void Camera3DMemento::restore() {
    if (object.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointerException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to create memento!");
    }
    sptr<Camera3D> p(object);
    p->width = width;
    p->height = height;

    p->origin = origin;
    p->viewUpVector = viewUpVector;
    p->deepVector = deepVector;
    p->bind = bind;

    p->nearCutter = nearCutter;
    p->farCutter = farCutter;
    p->screen = screen;

    p->defineProjectionTransformer();
    p->defineFrustrum();
    p->defineAxisTransformer();
}