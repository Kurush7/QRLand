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
    rotated_origin = origin;
    bind = Vector3D();

    defineFrustrum();
    defineAxisTransformer();
    defineProjectionTransformer();
}

void Camera3D::move(const Vector3D &move) {
    auto cr = MoveTransformer3DCreator (move);
    auto t = cr.create();
    bind = t->transform(bind);

    defineAxisTransformer();
}

void Camera3D::scale(float sx, float sy) {
    if (sx < QREPS || sy < QREPS)
        throw QRBadParamException(__FILE__, __LINE__, __TIME__, "negative scale for camera");
    width *= sx;
    height *= sy;

    defineFrustrum();
}

void Camera3D::scale(float scale) {
    if (scale < QREPS)
        throw QRBadParamException(__FILE__, __LINE__, __TIME__, "negative scale for camera");
    width *= scale;
    height *= scale;

    defineFrustrum();
}

void Camera3D::rotate(const Vector3D &rotate) {
    auto cr = RotateTransformer3DCreator (rotate);
    auto t = cr.create();

    // rotate around zeroPoint
    rotated_origin = t->transform(rotated_origin);
    auto localZero = t->transform(ZeroVector);
    auto yPoint = t->transform(viewUpVector);
    deepVector = lenNorm(localZero - rotated_origin);
    viewUpVector = lenNorm(yPoint - localZero);

    // rotate around self
    /*bind = t->transform(bind);
    deepVector = lenNorm(bind - origin);
    auto localZero = t->transform(ZeroVector);
    viewUpVector = lenNorm(t->transform(viewUpVector) - localZero);*/

    defineAxisTransformer();
    cout << axisTransformer->getMatrix() << '\n';
}

void Camera3D::defineFrustrum() {
    // todo scalar > 0: outside the pyramid
    // front-back
    frustrum.reserve(6);
    frustrum[0] = Vector3D{0,0,-1, origin[2] + nearCutter};
    frustrum[1] = Vector3D{0,0,-1, origin[2] + farCutter};
    // side-left-right
    frustrum[2] = Vector3D{-2*(origin[2]+screen)/width,0,1,0};  // right
    frustrum[3] = Vector3D{2*(origin[2]+screen)/width,0,1,0};   // left
    // up-down
    frustrum[4] = Vector3D{0,-2*(origin[2]+screen)/height,1,0};
    frustrum[5] = Vector3D{0,2*(origin[2]+screen)/height,1,0};

    frustrum.setSize(6);

    cout << "camera settings:\n";
    cout << "\tnear: " << nearCutter << '\n';
    cout << "\tfar: " << farCutter << '\n';
    cout << "\tscreen: " << screen << '\n';
    cout << "\twidth: " << width << '\n';
    cout << "\theight: " << height << '\n';
    cout << "\torigin: " << origin << '\n';
    cout << "\tbind: " << bind << '\n';
    Vector3D in_test({0,0,(nearCutter+farCutter)/2+origin[2],1});
    for (int i = 0; i < 6; ++i) {    // 0 & 1 not needed, for func will destroy 1
        frustrum[i] = len3Norm(frustrum[i]);
        if (scalar(frustrum[i], in_test) < 0) // inside values are  > 0
            frustrum[i] = -1 * frustrum[i];
        cout << "frustrum: " << frustrum[i] << '\n';
    }
}

void Camera3D::defineAxisTransformer() {
    // todo no origin given to transformer
    axisTransformer = sptr<QRTransformer3D>(new AxisChangeTransformer(
            lenNorm(viewUpVector*deepVector), viewUpVector, deepVector, bind+origin));
}

void Camera3D::defineProjectionTransformer() {
    projector = ProjectionTransformer3DCreator(0,0,screen).create();
}

int Camera3D::isVisibleSphere(const Vector3D &c, float rad) {
    Vector3D center {c[0], c[1], c[2], 1};
    center = axisTransformer->transform(center);
    float x;
    int good_cnt=0;
    cout << "model sphere: " << c << ' ' << center << ' ' << rad << '\n';
    cout << "camera cutting: ";
    for (int i = 0; i < 6; ++i) {
        x = scalar(center, frustrum[i]);
        cout << x << " ";
        if (x < -rad - QREPS) return 0;
        if (x > rad + QREPS) good_cnt++;
    }
    cout << '\n';
    if (good_cnt == 6) return 1;
    return 2;
}

bool Camera3D::isFrontFace(const Vector3D &normal) {
    // todo normal is already transformed into camera's coordinates!!!!!!
    return normal[2] < QREPS;
}