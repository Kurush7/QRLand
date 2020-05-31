//
// Created by kurush on 31.05.2020.
//

#include "SceneBuilder.h"

Scene3DBuilder::Scene3DBuilder (std::shared_ptr<AbstractObject3DFactory> f)
        :factory(f) {}

bool Scene3DBuilder::makeCamera() {
    camera = factory->createCamera(100, 100, Vector3D(0,0,0));
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