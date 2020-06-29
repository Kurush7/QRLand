//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_COMMAND_H
#define BIG3DFLUFFY_COMMAND_H

// todo file structure

#include "objects/objects.h"
#include "objects/visitors/QRVisitor.h"
#include "QRPainter.h"
#include "../drawManagers/SceneDrawMethod.h"
#include "../../objects/mementos/MultipleMementos.h"

class QRCommand {
public:
    virtual sptr<QRMemento> exec() = 0;
};
class SceneCommand: public QRCommand {
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
    DrawCommand(sptr<QRImage> painter, sptr<QRScene3D> s)
            :SceneCommand(s), painter(painter){}

    virtual sptr<QRMemento> exec();

private:
    sptr<QRImage> painter;
};

class SelectCommand: public SceneCommand {
public:
    SelectCommand(double x, double y, sptr<QRScene3D> &s)
            :SceneCommand(s), x(x), y(y) {}

    virtual sptr<QRMemento> exec();
private:
    double x, y;
};
class SetColorSelectionCommand: public SceneCommand {
public:
    SetColorSelectionCommand(ColorKeeper keeper, sptr<QRScene3D> &s)
            :SceneCommand(s), keeper(keeper){}

    virtual sptr<QRMemento> exec();

protected:
    sptr<QRTransformer3D> transformer;
    ColorKeeper keeper;
};
class DeleteSelectionCommand: public SceneCommand {
public:
    DeleteSelectionCommand(sptr<QRScene3D> &s)
            :SceneCommand(s){}

    virtual sptr<QRMemento> exec();
};


// todo fuck
class TransformSelectionCommand: public SceneCommand {
public:
    TransformSelectionCommand(sptr<QRTransformer3D> &trans, sptr<QRScene3D> &s)
            :SceneCommand(s), transformer(trans){}

    virtual sptr<QRMemento> exec();

protected:
    sptr<QRTransformer3D> transformer;
};
class MoveTransformSelectionCommand: public SceneCommand {
public:
    MoveTransformSelectionCommand(sptr<QRTransformer3D> &trans, sptr<QRScene3D> &s)
            :SceneCommand(s), transformer(trans){}

    virtual sptr<QRMemento> exec();

protected:
    sptr<QRTransformer3D> transformer;
};
class TransformCameraCommand: public SceneCommand {
public:
    TransformCameraCommand(sptr<QRTransformer3D> &trans, sptr<QRScene3D> &s)
            :SceneCommand(s), transformer(trans){}

    virtual sptr<QRMemento> exec();

protected:
    sptr<QRTransformer3D> transformer;
};
class ScaleCameraCommand: public TransformCameraCommand {
public:
    ScaleCameraCommand(sptr<QRTransformer3D> &trans, sptr<QRScene3D> &s)
            :TransformCameraCommand(trans, s) {}

    virtual sptr<QRMemento> exec();
};


#endif //BIG3DFLUFFY_COMMAND_H
