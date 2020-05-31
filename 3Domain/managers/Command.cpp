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