//
// Created by kurush on 29.05.2020.
//

#include "Lab3Facade.h"

using namespace std;

Lab3Facade::Lab3Facade() {
    manager = shared_ptr<BaseCommandManager> (new CommandManager());
    factory = shared_ptr<AbstractObject3DFactory> (new BasicObject3DFactory());

    auto director = SceneBuildDirector();
    auto builder = shared_ptr<BaseScene3DBuilder>(new Scene3DBuilder(factory));
    scene = director.build(builder);
}

void Lab3Facade::addModel(string filename) {
    auto data = shared_ptr<LoadData>(new FileLoadData(filename));
    auto source = shared_ptr<LoadSource>(new FileSource(data));
    auto loader = shared_ptr<BaseFrame3DLoader> (new Frame3DLoader(source, factory));
    auto director = FrameLoadDirector();
    auto command = shared_ptr<BaseCommand>(new AddModelCommand(director, loader, scene));
    manager->push(command);
}

void Lab3Facade::draw(std::shared_ptr<Painter> painter) {
    auto command = shared_ptr<BaseCommand>(new DrawCommand(painter, scene));
    manager->push(command);
    manager->execAll();
}