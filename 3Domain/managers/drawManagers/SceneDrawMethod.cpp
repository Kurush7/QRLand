//
// Created by kurush on 31.05.2020.
//

#include "SceneDrawMethod.h"


void SceneDrawMethod::draw(sptr<QRScene3D> s, sptr<QRPainter> p) {
    scene = s;
    painter = p;

    collectMetaData();
    collectPlainData();
    parsePlainData();
    paint();
}
void SceneDrawMethod::collectMetaData() {
    transformer = scene->getActiveCamera()->getProjectionTransformer();
    //cout << transformer->getMatrix();
}
void SceneDrawMethod::collectPlainData() {
    auto visitor = sptr<QRVisitor>(new DrawVisitor(transformer, drawableData, painter));
    for (auto object: scene->getObjects())
        object->acceptVisitor(visitor);
}
void SceneDrawMethod::parsePlainData() {
    auto c = scene->getActiveCamera();
    auto b = c->getBind();
    painter->setFrame(b[0], b[1], c->getWidth(), c->getHeight());

    // todo parse invisible for camera
    /*auto visitor = new HideInvisibleDrawMethodVisitor(c->getOrigin()->getPoint()[2]);
    cout << "origin-z: " << c->getOrigin()->getPoint()[2] << '\n';
    auto ptr = sptr<BaseDrawMethodVisitor>(visitor);
    for (size_t i = 0; i < drawableData.len(); ++i) {
        drawableData[i]->acceptVisitor(ptr);
        if (!visitor->visible) {
            drawableData.pop(i);
            i--;
        }
    }*/
}
void SceneDrawMethod::paint() {
    painter->reset();
    auto visitor = sptr<QRDrawVisitor> (new DrawMethodVisitor(painter));
    for (auto data: drawableData)
        data->acceptVisitor(visitor);

    painter->paint();
}