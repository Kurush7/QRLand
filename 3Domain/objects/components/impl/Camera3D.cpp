//
// Created by kurush on 28.06.2020.
//

#include "Camera3D.h"
#include "objects/mementos/QRCamera3DMemento.h"

uptr<QRMemento> Camera3D::save() {
    return uptr<QRMemento>(new Camera3DMemento(p));
}

Camera3D::Camera3D(float w, float h, float n, float f, const Vector3D &pos, const Vector3D &rot, bool _selfRotate, float zDist)
                   : QRCamera3D(w,h), screen(n), nearCutter(n), farCutter(f) {
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
    auto old_bind = bind;
    bind = t->transform(bind);

    if (!selfRotate) defineAxisTransformer();
    else {
        auto t = sptr<QRTransformer3D>(MoveTransformer3DCreator(bind-old_bind).create());
        t->accumulate(axisTransformer->getMatrix());
        axisTransformer = t;
    }

}

void Camera3D::scale(float sx, float sy) {
    if (sx < QREPS || sy < QREPS)
        throw QRBadParamException(__FILE__, __LINE__, __TIME__, "negative scale for camera");
    width *= sx;
    height *= sy;

    defineFrustrum();
}

void Camera3D::scale(float scale) {
    // todo hardcode
    scale = max(scale, QREPS*10);
    if (scale < QREPS)
        throw QRBadParamException(__FILE__, __LINE__, __TIME__, "negative scale for camera");
    width *= scale;
    height *= scale;

    defineFrustrum();
}

void Camera3D::rotate(const Vector3D &rotate) {
    if (!selfRotate) {
        // rotate around zeroPoint
        auto cr = RotateTransformer3DCreator(rotate);
        auto t = cr.create();
        worldCoordsOrigin = t->transform(worldCoordsOrigin);
        auto localZero = t->transform(ZeroVector);
        auto yPoint = t->transform(viewUpVector);

        deepVector = lenNorm(localZero - worldCoordsOrigin);
        viewUpVector = lenNorm(yPoint - localZero);

        defineAxisTransformer();
    }
    else {
        // rotate around origin point. rotation is in camera's coords, then accumulation
        auto cr = RotateTransformer3DCreator (rotate);
        auto t = cr.create();
        auto new_deepVector = t->transform(ZVector);
        new_deepVector[3] = 0;
        new_deepVector = lenNorm(new_deepVector);

        auto new_viewUpVector = t->transform(YVector);
        new_viewUpVector[3] = 0;
        new_viewUpVector = lenNorm(new_viewUpVector);

        auto new_axisTransformer  = sptr<QRTransformer3D>(new AxisChangeTransformer(
                lenNorm(new_viewUpVector*new_deepVector), new_viewUpVector,
                new_deepVector, ZeroVector));

        deepVector = new_axisTransformer->transform(deepVector);
        deepVector[3] = 0;
        deepVector = lenNorm(deepVector);

        viewUpVector = new_axisTransformer->transform(viewUpVector);
        viewUpVector[3] = 0;
        viewUpVector = lenNorm(viewUpVector);

        axisTransformer->accumulate(new_axisTransformer->getMatrix());
    }
}

void Camera3D::defineFrustrum() {
    // todo scalar > 0: outside the pyramid
    // front-back
    frustrum.reserve(6);
    frustrum[0] = Vector3D{0,0,-1, nearCutter};
    frustrum[1] = Vector3D{0,0,-1, farCutter};
    // side-left-right
    frustrum[2] = Vector3D{-2*(nearCutter)/width,0,1,0};  // right
    frustrum[3] = Vector3D{2*(nearCutter)/width,0,1,0};   // left
    // up-down
    frustrum[4] = Vector3D{0,1,0,0};
    frustrum[5] = Vector3D{0,nearCutter/height,-1,0};

    frustrum.setSize(6);

    /*cout << "camera settings:\n";
    cout << "\tnear: " << nearCutter << '\n';
    cout << "\tfar: " << farCutter << '\n';
    cout << "\tscreen: " << screen << '\n';
    cout << "\twidth: " << width << '\n';
    cout << "\theight: " << height << '\n';
    cout << "\torigin: " << origin << '\n';
    cout << "\tbind: " << bind << '\n';*/
    Vector3D in_test({0,height/2,(nearCutter+farCutter)/2+origin[2],1});
    for (int i = 0; i < frustrum.getSize(); ++i) {    // 0 & 1 not needed, for func will destroy 1
        frustrum[i] = len3Norm(frustrum[i]);
        if (scalar(frustrum[i], in_test) < 0) // inside values are  > 0
            frustrum[i] = -1 * frustrum[i];
        //cout << "frustrum: " << frustrum[i] << '\n';
    }
}

void Camera3D::defineAxisTransformer() {
    cout << "origins:\nox= " << lenNorm(viewUpVector*deepVector) << "\noy= " << viewUpVector << '\n';
    cout << "oz= " << deepVector <<
    "\norigin= " << bind+origin << "  (origin: " << origin << " bind: " << bind << " )\n";

    axisTransformer = sptr<QRTransformer3D>(new AxisChangeTransformer(
            lenNorm(viewUpVector*deepVector), viewUpVector,
            deepVector, bind+origin));

    cout << "new transformer:\n" << axisTransformer->getMatrix() << '\n';
}

void Camera3D::defineProjectionTransformer() {
    projector = ProjectionTransformer3DCreator(screen).create();
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
    //normal must be already transformed into camera's coordinates!!!!!!
    return normal[2] < QREPS;
}