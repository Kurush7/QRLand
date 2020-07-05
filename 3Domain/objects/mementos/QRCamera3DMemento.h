//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_QRCAMERA3DMEMENTO_H
#define BIG3DFLUFFY_QRCAMERA3DMEMENTO_H

#include "QRMemento.h"
#include "../components/interfaces/QRCamera3D.h"
#include "../components/impl/Camera3D.h"

// todo works not with abstract camera, but with concrete impl
class Camera3DMemento: public QRMemento {
public:
    Camera3DMemento(sptr<Camera3D>);
    virtual void restore();

private:
    wptr<Camera3D> object;
    Vector3D origin, viewUpVector, deepVector, bind;
    float nearCutter, farCutter, screen;
    float width, height;
};

#endif //BIG3DFLUFFY_QRCAMERA3DMEMENTO_H
