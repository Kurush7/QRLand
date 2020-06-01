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

class TransformCameraCommand: public SceneCommand {
public:
    TransformCameraCommand(std::shared_ptr<BaseTransformer3D> &trans, std::shared_ptr<BaseScene3D> &s)
            :SceneCommand(s), transformer(trans){}

    virtual std::shared_ptr<Memento> exec();

protected:
    std::shared_ptr<BaseTransformer3D> transformer;
};

class ScaleCameraCommand: public TransformCameraCommand {
public:
    ScaleCameraCommand(std::shared_ptr<BaseTransformer3D> &trans, std::shared_ptr<BaseScene3D> &s)
            :TransformCameraCommand(trans, s) {}

    virtual std::shared_ptr<Memento> exec();
};

class SelectCommand: public SceneCommand {
public:
    SelectCommand(double x, double y, std::shared_ptr<BaseScene3D> &s)
            :SceneCommand(s), x(x), y(y) {}

    virtual std::shared_ptr<Memento> exec();
private:
    double x, y;
};


class TransformSelectionCommand: public SceneCommand {
public:
    TransformSelectionCommand(std::shared_ptr<BaseTransformer3D> &trans, std::shared_ptr<BaseScene3D> &s)
            :SceneCommand(s), transformer(trans){}

    virtual std::shared_ptr<Memento> exec();

protected:
    std::shared_ptr<BaseTransformer3D> transformer;
};

class MoveTransformSelectionCommand: public SceneCommand {
public:
    MoveTransformSelectionCommand(std::shared_ptr<BaseTransformer3D> &trans, std::shared_ptr<BaseScene3D> &s)
            :SceneCommand(s), transformer(trans){}

    virtual std::shared_ptr<Memento> exec();

protected:
    std::shared_ptr<BaseTransformer3D> transformer;
};

class SetColorSelectionCommand: public SceneCommand {
public:
    SetColorSelectionCommand(ColorKeeper keeper, std::shared_ptr<BaseScene3D> &s)
            :SceneCommand(s), keeper(keeper){}

    virtual std::shared_ptr<Memento> exec();

protected:
    std::shared_ptr<BaseTransformer3D> transformer;
    ColorKeeper keeper;
};

class DeleteSelectionCommand: public SceneCommand {
public:
    DeleteSelectionCommand(std::shared_ptr<BaseScene3D> &s)
            :SceneCommand(s){}

    virtual std::shared_ptr<Memento> exec();
};




#endif //BIG3DFLUFFY_COMMAND_H
