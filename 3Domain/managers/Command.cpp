//
// Created by kurush on 31.05.2020.
//

#include "Command.h"

using namespace std;

shared_ptr<Memento> AddModelCommand::exec() {
    auto model = director.load(loader);
    auto mem = scene->save();
    scene->addObject(model);
    return mem;
}

shared_ptr<Memento> DrawCommand::exec() {
    SceneDrawMethod draw;
    draw.draw(scene, painter);
    return nullptr;
}

shared_ptr<Memento> TransformCameraCommand::exec() {
    auto camera = scene->getActiveCamera();
    auto mem = camera->save();

    auto visitor = shared_ptr<Visitor>(new TransformVisitor(transformer));
    camera->acceptVisitor(visitor);
    return mem;
}

shared_ptr<Memento> ScaleCameraCommand::exec() {
    auto camera = scene->getActiveCamera();
    auto mem = camera->save();

    auto visitor = shared_ptr<Visitor>(new ScaleCameraVisitor(transformer));
    camera->acceptVisitor(visitor);
    return mem;
}

shared_ptr<Memento> SelectCommand::exec() {
    auto camera = scene->getActiveCamera();

    auto transformer = shared_ptr<BaseTransformer3D>(camera->getProjectionTransformer());
    auto visitor = new SelectionVisitor(x,y, transformer);
    auto pnt =  shared_ptr<Visitor>(visitor);
    for (auto it = scene->getObjects().begin(); it != scene->getObjects().end(); ++it) {
        visitor->is_selected = false;
        it->get()->acceptVisitor(pnt);
        if (visitor->is_selected) {
            scene->switchSelection(it);
        }
    }
    return nullptr;
}