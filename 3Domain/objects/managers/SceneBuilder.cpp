//
// Created by kurush on 31.05.2020.
//

#include "SceneBuilder.h"

Scene3DBuilder::Scene3DBuilder (sptr<QRObject3DFactory> f, float width, float height)
        :factory(f), w(width), h(height) {}

bool Scene3DBuilder::makeCamera() {
    camera = factory->createCamera(w, h, Vector3D(0,0,-10));
    stage++;
    return true;
}

sptr<QRScene3D> Scene3DBuilder::getScene() {
    if (!isReady()) return nullptr;
    if (isBuilt) return scene;
    scene = sptr<QRScene3D>(new SimpleScene3D(camera));
    isBuilt = true;
    return scene;
}