//
// Created by kurush on 28.06.2020.
//

#include "Camera3D.h"

Camera3D::Camera3D(double w, double h, const Vector3D &o, double s,
                   double n, double f): QRCamera3D(w,h), origin(o), screen(s), nearCutter(n), farCutter(f) {
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
    origin = t->transform(origin);
    defineAxisTransformer();
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
    //cout << "matrix: " << t->getMatrix() << '\n';
    //cout << "bind: " << bind << '\n';
    //cout << "before rot: " << origin << ' ' << deepVector << ' ' << viewUpVector << '\n';
    origin = t->transform(origin + bind) - bind;
    auto localZero = t->transform(bind) - bind;
    auto yPoint = t->transform(bind + viewUpVector) - bind;
    deepVector = lenNorm(localZero - origin);
    viewUpVector = lenNorm(yPoint - localZero);
    //cout << "local: " << localZero << ' ' << yPoint  << '\n';
    //cout << "after rot: " << origin << ' ' << deepVector << ' ' << viewUpVector << '\n';
    defineAxisTransformer();
}

void Camera3D::defineFrustrum() {
    // todo scalar > 0: outside the pyramid
    // front-back
    frustrum[0] = Vector3D{0,0,-1, nearCutter};
    frustrum[1] = Vector3D{0,0,1, -farCutter};
    // side-left-right
    frustrum[2] = Vector3D{-2*screen/width,0,1,0};  // right
    frustrum[3] = Vector3D{2*screen/width,0,1,0};   // left
    // up-down
    frustrum[4] = Vector3D{0,-2*screen/height,1,0};
    frustrum[4] = Vector3D{0,2*screen/height,1,0};

    Vector3D in_test({0,0,(nearCutter+farCutter)/2,1});
    for (int i = 2; i < 6; ++i) {    // 0 & 1 not needed, for func will destroy 1
        frustrum[i] = lenNorm(frustrum[i]);
        if (scalar(frustrum[i], in_test) > 0)
            frustrum[i] = -1 * frustrum[i];
    }
}

void Camera3D::defineAxisTransformer() {
    axisTransformer = sptr<QRTransformer3D>(new AxisChangeTransformer(origin,
            viewUpVector*deepVector, viewUpVector, deepVector));
}

void Camera3D::defineProjectionTransformer() {
    projector = ProjectionTransformer3DCreator(0,0,screen).create();
}

bool Camera3D::isVisibleSphere(const Vector3D &c, double rad) {
    // todo check
    Vector3D center = axisTransformer->transform(c);
    center[3] = 1;
    for(int i = 0; i < 6; ++i)
        if(scalar(center, frustrum[i]) > rad)
            return false;
    return true;
}

bool Camera3D::isFrontFace(const Vector3D &normal) {
    // todo normal is already transformed!!!!!!
    /*Vector3D look({0,0,screen,0});
    // todo optimize
    if (scalar(normal, look) > 0)
        return false;
    return true;*/
    return normal[2] <= 0;

}