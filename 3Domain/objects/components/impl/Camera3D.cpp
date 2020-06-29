//
// Created by kurush on 28.06.2020.
//

#include "Camera3D.h"

Camera3D::Camera3D(double w, double h, const sptr<ProjectionImp> &p, const Vector3D &o, double s,
                   double n, double f): QRCamera3D(w,h,p), origin(o), screen(s), nearCutter(n), farCutter(f) {}

void Camera3D::move(const Vector3D &move) {
    auto cr = MoveTransformer3DCreator (move);
    auto t = cr.create();
    origin = t->transform(origin);
}

void Camera3D::scale(double sx, double sy) {
    if (sx < QREPS || sy < QREPS)
        throw QRBadParamException(__FILE__, __LINE__, __TIME__, "negative scale for camera");
    width *= sx;
    height *= sy;
}

void Camera3D::scale(double scale) {
    if (scale < QREPS)
        throw QRBadParamException(__FILE__, __LINE__, __TIME__, "negative scale for camera");
    width *= scale;
    height *= scale;
}

void Camera3D::rotate(const Vector3D &rotate) {
    auto cr = RotateTransformer3DCreator (rotate);
    auto t = cr.create();
    deepVector = t->transform(deepVector);
    viewUpVector = t->transform(viewUpVector);
}