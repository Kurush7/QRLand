//
// Created by kurush on 30.05.2020.
//

#include "Camera3D.h"

using namespace std;

unique_ptr<Memento> BaseCamera3D::save() {
    return unique_ptr<Memento>(new Camera3DMemento(p));
}

double BaseCamera3D::getWidth() const {
    return width;
}

void BaseCamera3D::setWidth(double width) {
    BaseCamera3D::width = width;
}

double BaseCamera3D::getHeight() const {
    return height;
}

void BaseCamera3D::setHeight(double height) {
    BaseCamera3D::height = height;
}

Camera3DMemento::Camera3DMemento(shared_ptr<BaseCamera3D> p) {
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
    shared_ptr<BaseCamera3D> p(object);
    p->setOrigin(origin);
    p->setWidth(width);
    p->setHeight(height);
    p->setProjector(projector);
}

// todo get point already
Camera3D::Camera3D(double w, double h, shared_ptr<ProjectionImp>pr, const Vector3D &_origin,
                       const Vector3D &viewUp, const Vector3D &_bind)
: BaseCamera3D(w, h, pr), origin(new QRPoint3D(_origin, _bind)), viewUpVector(viewUp) {
}

unique_ptr<BaseTransformer3D> Camera3D::getProjectionTransformer() {
    return projector->getTransformer(origin->getPoint(), origin->getBind(), viewUpVector);
}