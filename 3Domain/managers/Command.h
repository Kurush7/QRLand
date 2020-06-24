//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_COMMAND_H
#define BIG3DFLUFFY_COMMAND_H

#include "objects/objects.h"
#include "../Painter.h"
#include "SceneDrawMethod.h"

class BaseCommand {
public:
    virtual sptr<QRMemento> exec() = 0;
};

class SceneCommand: public BaseCommand {
public:
    SceneCommand(sptr<QRScene3D> s): scene(s) {}
protected:
    sptr<QRScene3D> scene;
};


class AddModelCommand: public SceneCommand {
public:
    AddModelCommand(FrameLoadDirector director, sptr<BaseFrame3DLoader> loader,
            sptr<QRScene3D> s)
    :SceneCommand(s), loader(loader), director(director){}

    virtual sptr<QRMemento> exec();

private:
    sptr<BaseFrame3DLoader> loader;
    FrameLoadDirector director;
};

class DrawCommand: public SceneCommand {
public:
    DrawCommand(sptr<Painter> painter, sptr<QRScene3D> s)
            :SceneCommand(s), painter(painter){}

    virtual sptr<QRMemento> exec();

private:
    sptr<Painter> painter;
};

class TransformCameraCommand: public SceneCommand {
public:
    TransformCameraCommand(sptr<BaseTransformer3D> &trans, sptr<QRScene3D> &s)
            :SceneCommand(s), transformer(trans){}

    virtual sptr<QRMemento> exec();

protected:
    sptr<BaseTransformer3D> transformer;
};

class ScaleCameraCommand: public TransformCameraCommand {
public:
    ScaleCameraCommand(sptr<BaseTransformer3D> &trans, sptr<QRScene3D> &s)
            :TransformCameraCommand(trans, s) {}

    virtual sptr<QRMemento> exec();
};

class SelectCommand: public SceneCommand {
public:
    SelectCommand(double x, double y, sptr<QRScene3D> &s)
            :SceneCommand(s), x(x), y(y) {}

    virtual sptr<QRMemento> exec();
private:
    double x, y;
};


class TransformSelectionCommand: public SceneCommand {
public:
    TransformSelectionCommand(sptr<BaseTransformer3D> &trans, sptr<QRScene3D> &s)
            :SceneCommand(s), transformer(trans){}

    virtual sptr<QRMemento> exec();

protected:
    sptr<BaseTransformer3D> transformer;
};

class MoveTransformSelectionCommand: public SceneCommand {
public:
    MoveTransformSelectionCommand(sptr<BaseTransformer3D> &trans, sptr<QRScene3D> &s)
            :SceneCommand(s), transformer(trans){}

    virtual sptr<QRMemento> exec();

protected:
    sptr<BaseTransformer3D> transformer;
};

class SetColorSelectionCommand: public SceneCommand {
public:
    SetColorSelectionCommand(ColorKeeper keeper, sptr<QRScene3D> &s)
            :SceneCommand(s), keeper(keeper){}

    virtual sptr<QRMemento> exec();

protected:
    sptr<BaseTransformer3D> transformer;
    ColorKeeper keeper;
};

class DeleteSelectionCommand: public SceneCommand {
public:
    DeleteSelectionCommand(sptr<QRScene3D> &s)
            :SceneCommand(s){}

    virtual sptr<QRMemento> exec();
};




#endif //BIG3DFLUFFY_COMMAND_H
