//
// Created by kurush on 31.05.2020.
//

#include "SceneDrawMethod.h"


void SceneDrawMethod::draw(std::shared_ptr<BaseScene3D> s, std::shared_ptr<Painter> p) {
    scene = s;
    painter = p;

    collectMetaData();
    collectPlainData();
    parsePlainData();
    paint();
}
void SceneDrawMethod::collectMetaData() {
    transformer = scene->getActiveCamera()->getProjectionTransformer();
    cout << transformer->getMatrix();
}
void SceneDrawMethod::collectPlainData() {
    auto visitor = shared_ptr<Visitor>(new DrawVisitor(transformer, drawableData));
    for (auto object: scene->getObjects())
        object->acceptVisitor(visitor);
}
void SceneDrawMethod::parsePlainData() {
    // todo invisible
}
void SceneDrawMethod::paint() {
    DrawMethodVisitor visitor(painter);
    for (auto data: drawableData)
        data->acceptVisitor(visitor);
}