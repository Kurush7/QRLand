//
// Created by kurush on 31.05.2020.
//

#include "Scene3D.h"

using namespace std;

uptr<QRMemento> BaseScene3D::save() {return uptr<QRMemento>(new SceneMemento(p));}

SceneMemento::SceneMemento(sptr<BaseScene3D> scene): scene(scene) {
        camera = scene->getActiveCamera();
        objects = scene->getObjects();
}

void SceneMemento::restore() {
    if (scene.expired())
        throw QRBadPointerException(__FILE__, __LINE__, __TIME__, "Failed to create memento!");
    scene.lock()->setObjects(objects);
}