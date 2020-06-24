//
// Created by kurush on 24.06.2020.
//

#include "QRSceneMemento.h"

QRSceneMemento::QRSceneMemento(sptr<QRScene3D> scene): scene(scene) {
    camera = scene->getActiveCamera();
    objects = scene->getObjects();
}

void QRSceneMemento::restore() {
    if (scene.expired())
        throw QRBadPointerException(__FILE__, __LINE__, __TIME__, "Failed to create memento!");
    scene.lock()->setObjects(objects);
}