//
// Created by kurush on 24.06.2020.
//

#include "SimpleCamera3D.h"

// todo get point already
SimpleCamera3D::SimpleCamera3D(double w, double h, sptr<ProjectionImp>pr, const Vector3D &_origin,
                               const Vector3D &viewUp, const Vector3D &_bind)
        : QRCamera3D(w, h, pr), origin(new SimplePoint3D(_origin, _bind)), viewUpVector(viewUp) {
}

uptr<BaseTransformer3D> SimpleCamera3D::getProjectionTransformer() {
    return projector->getTransformer(origin->getPoint(), origin->getBind(), viewUpVector);
}