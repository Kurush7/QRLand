//
// Created by kurush on 31.05.2020.
//

#include "Command.h"

using namespace std;

shared_ptr<Memento> AddModelCommand::exec() {
    auto model = director.load(loader);
    if (!model)
        throw QRBadSourceException(__FILE__, __LINE__, __TIME__, "failed to read model from file!");
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


shared_ptr<Memento> TransformSelectionCommand::exec() {
    auto mem = scene->save();
    // todo group memento with filling new memes
    auto visitor = shared_ptr<Visitor>(new TransformVisitor(transformer));
    for (auto x: scene->getSelection())
        x->acceptVisitor(visitor);
    return mem;
}

shared_ptr<Memento> MoveTransformSelectionCommand::exec() {
    auto mem = scene->save();
    // todo group memento with filling new memes
    auto visitor = shared_ptr<Visitor>(new MoveTransformVisitor(transformer));
    for (auto x: scene->getSelection())
        x->acceptVisitor(visitor);
    return mem;
}

shared_ptr<Memento> SetColorSelectionCommand::exec() {
    auto mem = scene->save();
    // todo group memento with filling new memes
    auto visitor = shared_ptr<Visitor>(new SetColorVisitor(keeper));
    for (auto x: scene->getSelection())
        x->acceptVisitor(visitor);
    return mem;
}


shared_ptr<Memento> DeleteSelectionCommand::exec() {
    auto mem = scene->save();
    QRVector<ObjectIterator> selected;
    // todo accurate delete: smart iterators & another system
    for (auto it = scene->getObjects().begin(); it != scene->getObjects().end(); ++it)
        if (it->get()->isSelected())
            selected.push_back(it);
    for (auto x: selected)
        scene->deleteObject(x);
    return mem;
}
