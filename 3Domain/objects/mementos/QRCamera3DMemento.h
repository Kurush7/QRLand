//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_QRCAMERA3DMEMENTO_H
#define BIG3DFLUFFY_QRCAMERA3DMEMENTO_H

#include "QRMemento.h"
#include "../components/interfaces/QRCamera3D.h"

class Camera3DMemento: public QRMemento {
public:
    Camera3DMemento(std::shared_ptr<QRCamera3D>);
    virtual void restore();

private:
    std::weak_ptr<QRCamera3D> object;
    std::shared_ptr<QRPoint3D> origin;
    double width, height;
    std::shared_ptr<ProjectionImp> projector;
};

#endif //BIG3DFLUFFY_QRCAMERA3DMEMENTO_H
