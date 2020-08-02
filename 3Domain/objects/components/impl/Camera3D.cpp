//
// Created by kurush on 28.06.2020.
//

#include "Camera3D.h"
#include "objects/mementos/QRCamera3DMemento.h"

uptr<QRMemento> Camera3D::save() {
    return uptr<QRMemento>(new Camera3DMemento(p));
}

Camera3D::Camera3D(float w, float h, const Vector3D &o, float s,
                   float n, float f): QRCamera3D(w,h), origin(o), screen(s), nearCutter(n), farCutter(f) {
    p = sptr<Camera3D>(this, [](void *ptr){});
    viewUpVector = {0,1,0,0};
    deepVector = {0,0,1,0};
    bind = origin;
    defineFrustrum();
    defineAxisTransformer();
    defineProjectionTransformer();
}

void Camera3D::move(const Vector3D &move) {
    auto cr = MoveTransformer3DCreator (move);
    auto t = cr.create();
    bind = t->transform(bind);
    // todo no moving.....
    defineAxisTransformer();
}

void Camera3D::scale(float sx, float sy) {
    if (sx < QREPS || sy < QREPS)
        throw QRBadParamException(__FILE__, __LINE__, __TIME__, "negative scale for camera");
    width *= sx;
    height *= sy;
}

void Camera3D::scale(float scale) {
    if (scale < QREPS)
        throw QRBadParamException(__FILE__, __LINE__, __TIME__, "negative scale for camera");
    width *= scale;
    height *= scale;
}

void Camera3D::rotate(const Vector3D &rotate) {
    auto cr = RotateTransformer3DCreator (rotate);
    auto t = cr.create();
    cout << "rotate by: " << rotate << '\n';

    origin = t->transform(origin);
    auto localZero = t->transform(ZeroVector);
    auto yPoint = t->transform(viewUpVector);
    deepVector = lenNorm(localZero - origin);
    viewUpVector = lenNorm(yPoint - localZero);

    defineAxisTransformer();
}

void Camera3D::defineFrustrum() {
    // todo scalar > 0: outside the pyramid
    // front-back
    frustrum[0] = Vector3D{0,0,-1, origin[2] + nearCutter};
    frustrum[1] = Vector3D{0,0,1, origin[2] - farCutter};
    // side-left-right
    frustrum[2] = Vector3D{-2*(origin[2]+screen)/width,0,1,0};  // right
    frustrum[3] = Vector3D{2*(origin[2]+screen)/width,0,1,0};   // left
    // up-down
    frustrum[4] = Vector3D{0,-2*(origin[2]+screen)/height,1,0};
    frustrum[4] = Vector3D{0,2*(origin[2]+screen)/height,1,0};

    Vector3D in_test({0,0,(nearCutter+farCutter)/2+origin[2],1});
    for (int i = 2; i < 6; ++i) {    // 0 & 1 not needed, for func will destroy 1
        frustrum[i] = lenNorm(frustrum[i]);
        if (scalar(frustrum[i], in_test) > 0)
            frustrum[i] = -1 * frustrum[i];
    }
}

void Camera3D::defineAxisTransformer() {
    // todo no origin given to transformer
    axisTransformer = sptr<QRTransformer3D>(new AxisChangeTransformer(
            lenNorm(viewUpVector*deepVector), viewUpVector, deepVector, bind));
}

void Camera3D::defineProjectionTransformer() {
    projector = ProjectionTransformer3DCreator(0,0,screen).create();
}

bool Camera3D::isVisibleSphere(const Vector3D &c, float rad) {
    Vector3D center {c[0], c[1], c[2], 1};  // todo 1 is not necessary here.... for now (see to matrix*vector todo)
    center = axisTransformer->transform(center);
    for(int i = 0; i < 6; ++i)
        if(scalar(center, frustrum[i]) > rad)
            return false;
    return true;
}

bool Camera3D::isFrontFace(const Vector3D &normal) {
    // todo normal is already transformed into camera's coordinates!!!!!!
    return normal[2] < QREPS;
}