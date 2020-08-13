//
// Created by kurush on 28.06.2020.
//

#include "Camera3D.h"
#include "objects/mementos/QRCamera3DMemento.h"

uptr<QRMemento> Camera3D::save() {
    return uptr<QRMemento>(new Camera3DMemento(p));
}

Camera3D::Camera3D(float w, float h, float zDist, float s,
                   float n, float f, const Vector3D &pos, const Vector3D &rot, bool _selfRotate)
                   : QRCamera3D(w,h), screen(s), nearCutter(n), farCutter(f) {
    p = sptr<Camera3D>(this, [](void *ptr){});
    viewUpVector = YVector;
    deepVector = ZVector;
    origin = Vector3D(0, 0, zDist, 0);
    worldCoordsOrigin = origin;
    bind = ZeroVector;

    /*
    auto t =  AxisChangeTransformer(lenNorm(viewUp*view), lenNorm(viewUp), lenNorm(view), orig);
    bind = t.transform(bind);
    viewUpVector = lenNorm(t.transform(viewUpVector) - bind);
    deepVector = lenNorm(t.transform(deepVector) - bind);*/

    defineFrustrum();
    defineAxisTransformer();
    defineProjectionTransformer();

    rotate(rot);
    move(pos);
    selfRotate = _selfRotate;
}

void Camera3D::move(const Vector3D &move) {
    // todo here when self-rotate
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
    if (!selfRotate) {
        worldCoordsOrigin = t->transform(worldCoordsOrigin);
        auto localZero = t->transform(ZeroVector);
        auto yPoint = t->transform(viewUpVector);
        deepVector = lenNorm(localZero - worldCoordsOrigin);
        viewUpVector = lenNorm(yPoint - localZero);
    }

    else {
        // rotate around self

        cout << "rotate by: " << rotate <<  ", origin: " << origin << '\n';
        cout << bind << ' ' << deepVector << ' ' << viewUpVector << '\n';
        //bind = t->transform(bind-origin) + origin;
        //bind[3] = 0;

        auto world_origin = bind + deepVector*origin[2];
        world_origin[3] = 0;

        deepVector = t->transform(deepVector);
        deepVector[3] = 0;
        deepVector = lenNorm(deepVector);

        viewUpVector = t->transform(viewUpVector);
        viewUpVector[3] = 0;
        viewUpVector = lenNorm(viewUpVector);

        cout << origin[2] << ' ' << deepVector << ' ' << world_origin << '\n';
        bind = world_origin - origin[2]*deepVector;
        bind[3] = 0;

    }

    cout << bind << ' ' << deepVector << ' ' << viewUpVector << '\n';
    cout << "world origin: " << bind + deepVector*origin[2] << '\n';

    defineAxisTransformer();
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

    /*cout << "camera settings:\n";
    cout << "\tnear: " << nearCutter << '\n';
    cout << "\tfar: " << farCutter << '\n';
    cout << "\tscreen: " << screen << '\n';
    cout << "\twidth: " << width << '\n';
    cout << "\theight: " << height << '\n';
    cout << "\torigin: " << origin << '\n';
    cout << "\tbind: " << bind << '\n';*/
    Vector3D in_test({0,0,(nearCutter+farCutter)/2+origin[2],1});
    for (int i = 0; i < 6; ++i) {    // 0 & 1 not needed, for func will destroy 1
        frustrum[i] = len3Norm(frustrum[i]);
        if (scalar(frustrum[i], in_test) < 0) // inside values are  > 0
            frustrum[i] = -1 * frustrum[i];
        //cout << "frustrum: " << frustrum[i] << '\n';
    }
}

void Camera3D::defineAxisTransformer() {
    cout << "origins:\nox= " << lenNorm(viewUpVector*deepVector) << "\noy= " << viewUpVector << '\n';
    cout << "oz= " << deepVector << "\norigin= " << bind+origin << '\n';
    axisTransformer = sptr<QRTransformer3D>(new AxisChangeTransformer(
            lenNorm(viewUpVector*deepVector), viewUpVector,
            deepVector, bind+origin));
    cout << "new transformer:\n" << axisTransformer->getMatrix() << '\n';
}

void Camera3D::defineProjectionTransformer() {
    projector = ProjectionTransformer3DCreator(0,0,screen).create();
}

int Camera3D::isVisibleSphere(const Vector3D &c, float rad) {
    Vector3D center {c[0], c[1], c[2], 1};
    center = axisTransformer->transform(center);
    float x;
    int good_cnt=0;
    for (int i = 0; i < 6; ++i) {
        x = scalar(center, frustrum[i]);
        if (x < -rad - QREPS) return 0;
        if (x > rad + QREPS) good_cnt++;
    }
    if (good_cnt == 6) return 1;
    return 2;
}

bool Camera3D::isFrontFace(const Vector3D &normal) {
    // todo normal is already transformed into camera's coordinates!!!!!!
    return normal[2] < QREPS;
}