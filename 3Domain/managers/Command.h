//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_COMMAND_H
#define BIG3DFLUFFY_COMMAND_H

#include "../objects/QRObjects.h"
#include "../Painter.h"
#include "SceneDrawMethod.h"

class BaseCommand {
public:
    virtual std::shared_ptr<Memento> exec() = 0;
};

class SceneCommand: public BaseCommand {
public:
    SceneCommand(std::shared_ptr<BaseScene3D> s): scene(s) {}
protected:
    std::shared_ptr<BaseScene3D> scene;
};


class AddModelCommand: public SceneCommand {
public:
    AddModelCommand(FrameLoadDirector director, std::shared_ptr<BaseFrame3DLoader> loader,
            std::shared_ptr<BaseScene3D> s)
    :SceneCommand(s), loader(loader), director(director){}

    virtual std::shared_ptr<Memento> exec();

private:
    std::shared_ptr<BaseFrame3DLoader> loader;
    FrameLoadDirector director;
};

class DrawCommand: public SceneCommand {
public:
    DrawCommand(std::shared_ptr<Painter> painter, std::shared_ptr<BaseScene3D> s)
            :SceneCommand(s), painter(painter){}

    virtual std::shared_ptr<Memento> exec();

private:
    std::shared_ptr<Painter> painter;
};


#endif //BIG3DFLUFFY_COMMAND_H
