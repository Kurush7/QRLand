//
// Created by kurush on 31.05.2020.
//

#include "Command.h"
#include "mementos/MultipleMementos.h"

using namespace std;

sptr<QRMemento> AddModelCommand::exec() {
    auto model = director.load(loader);
    if (!model)
        throw QRBadSourceException(__FILE__, __LINE__, __TIME__, "failed to read model from file!");
    auto mem = scene->save();
    scene->addObject(model);
    return mem;
}

sptr<QRMemento> DrawCommand::exec() {
    SceneDrawMethod draw;
    draw.draw(scene, painter);
    return nullptr;
}

sptr<QRMemento> TransformCameraCommand::exec() {
    auto camera = scene->getActiveCamera();
    auto mem = camera->save();

    auto visitor = sptr<QRVisitor>(new TransformVisitor(transformer));
    camera->acceptVisitor(visitor);
    return mem;
}

sptr<QRMemento> ScaleCameraCommand::exec() {
    auto camera = scene->getActiveCamera();
    auto mem = camera->save();

    auto visitor = sptr<QRVisitor>(new ScaleCameraVisitor(transformer));
    camera->acceptVisitor(visitor);
    return mem;
}

sptr<QRMemento> SelectCommand::exec() {
    auto camera = scene->getActiveCamera();

    auto transformer = sptr<BaseTransformer3D>(camera->getProjectionTransformer());
    auto visitor = new SelectionVisitor(x,y, transformer);
    auto pnt =  sptr<QRVisitor>(visitor);
    for (auto it = scene->getObjects().begin(); it != scene->getObjects().end(); ++it) {
        visitor->is_selected = false;
        it->get()->acceptVisitor(pnt);
        if (visitor->is_selected) {
            scene->switchSelection(it);
        }
    }
    return nullptr;
}


sptr<QRMemento> TransformSelectionCommand::exec() {
    auto memes = new MementoAccumulator();

    auto visitor = sptr<QRVisitor>(new TransformVisitor(transformer));
    for (auto x: scene->getSelection()) {
        memes->add(x->save());
        x->acceptVisitor(visitor);
    }
    return sptr<QRMemento>(memes);
}

sptr<QRMemento> MoveTransformSelectionCommand::exec() {
    auto memes = new MementoAccumulator();

    auto visitor = sptr<QRVisitor>(new MoveTransformVisitor(transformer));
    for (auto x: scene->getSelection()) {
        memes->add(x->save());
        x->acceptVisitor(visitor);
    }
    return sptr<QRMemento>(memes);
}

sptr<QRMemento> SetColorSelectionCommand::exec() {
    auto memes = new MementoAccumulator();

    auto visitor = sptr<QRVisitor>(new SetColorVisitor(keeper));
    for (auto x: scene->getSelection()) {
        memes->add(x->save());
        x->acceptVisitor(visitor);
    }
    return sptr<QRMemento>(memes);
}


sptr<QRMemento> DeleteSelectionCommand::exec() {
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
