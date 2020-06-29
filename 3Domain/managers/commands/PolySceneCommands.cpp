//
// Created by kurush on 29.06.2020.
//

#include "PolySceneCommands.h"

sptr<QRMemento> MoveCameraCmd::exec() {
    auto camera = scene->getActiveCamera();
    auto mem = camera->save();
    camera->move(vec);
    return mem;
}

sptr<QRMemento> ScaleCameraCmd::exec() {
    auto camera = scene->getActiveCamera();
    auto mem = camera->save();
    camera->scale(vec[0]);  // todo both v[0] and v[1]
    return mem;
}

sptr<QRMemento> RotateCameraCmd::exec() {
    auto camera = scene->getActiveCamera();
    auto mem = camera->save();
    camera->rotate(vec);
    return mem;
}

sptr<QRMemento> AddModelCmd::exec() {
    auto mem = scene->save();
    scene->addModel(cr->create(), v);
    return mem;
}
