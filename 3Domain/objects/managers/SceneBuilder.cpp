//
// Created by kurush on 31.05.2020.
//

#include "SceneBuilder.h"

Scene3DBuilder::Scene3DBuilder (std::shared_ptr<AbstractObject3DFactory> f, double width, double height)
        :factory(f), w(width), h(height) {}

bool Scene3DBuilder::makeCamera() {
    camera = factory->createCamera(w, h, Vector3D(0,0,-10));
    stage++;
    return true;
}

std::shared_ptr<BaseScene3D> Scene3DBuilder::getScene() {
    if (!isReady()) return nullptr;
    if (isBuilt) return scene;
    scene = shared_ptr<BaseScene3D>(new Scene3D(camera));
    isBuilt = true;
    return scene;
}