//
// Created by kurush on 28.06.2020.
//

#include "QRPolySceneMemento.h"

QRPolySceneMemento::QRPolySceneMemento(sptr<QRPolyScene3D> scene)
: scene(scene), memes(new MementoAccumulator) {
    camera = scene->getActiveCamera();
    models = scene->getModels();
    memes->add(camera->save());
    for (auto x: models)
        memes->add(x.fst->save());
}

void QRPolySceneMemento::restore() {
    if (scene.expired())
        throw QRBadPointerException(__FILE__, __LINE__, __TIME__, "Failed to create memento!");
    auto s = scene.lock();
    s->setModels(models.begin());
    //todo here a fuck s->setActiveCamera(camera);
    memes->restore();
}