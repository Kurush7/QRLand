//
// Created by kurush on 29.06.2020.
//

#ifndef BIG3DFLUFFY_POLYSCENECREATOR_H
#define BIG3DFLUFFY_POLYSCENECREATOR_H

#include "PolyScene3D.h"
#include "objects/components/impl.h"

class QRPolySceneCreator {
public:
    virtual uptr<QRPolyScene3D> create() = 0;
};

class PolySceneCreator: public QRPolySceneCreator {
public:
    virtual uptr<QRPolyScene3D> create() {
        auto cr = CameraCreator(5, 5);
        auto cam = sptr<QRCamera3D>(cr.create(10, 10, Vector3D(0,0,-20)));
        auto scene = uptr<QRPolyScene3D>(new PolyScene3D(cam));
        auto light = sptr<QRLight>(new BaseLight(Vector3D(0,0,-100)));
        scene->addLight(light);
        return scene;
    }
};


#endif //BIG3DFLUFFY_POLYSCENECREATOR_H
