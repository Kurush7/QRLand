//
// Created by kurush on 29.06.2020.
//

#ifndef BIG3DFLUFFY_POLYSCENECOMMANDS_H
#define BIG3DFLUFFY_POLYSCENECOMMANDS_H

#include "QRCommandInterface.h"
#include "objects/composites/impl.h"
#include "../../renderStuff/QRender.h"

class MoveCameraCmd: public PolySceneCommand {
public:
    MoveCameraCmd(const Vector3D &vec, const sptr<QRPolyScene3D> &s)
    :PolySceneCommand(s), vec(vec) {}

    virtual sptr<QRMemento> exec();
private:
    Vector3D vec;
};

class ScaleCameraCmd: public PolySceneCommand {
public:
    ScaleCameraCmd(const Vector3D &vec, const sptr<QRPolyScene3D> &s)
            :PolySceneCommand(s), vec(vec) {}

    virtual sptr<QRMemento> exec();
private:
    Vector3D vec;
};

class RotateCameraCmd: public PolySceneCommand {
public:
    RotateCameraCmd(const Vector3D &vec, const sptr<QRPolyScene3D> &s)
            :PolySceneCommand(s), vec(vec) {}

    virtual sptr<QRMemento> exec();
private:
    Vector3D vec;
};

class AddModelCmd: public PolySceneCommand {
public:
    AddModelCmd(const sptr<QRPolyModelCreator> &cr, const Vector3D &v, const sptr<QRPolyScene3D> &s)
            :PolySceneCommand(s), cr(cr), v(v) {}

    virtual sptr<QRMemento> exec();
private:
    sptr<QRPolyModelCreator> cr;
    Vector3D v;
};

class RenderCmd: public PolySceneCommand {
public:
    RenderCmd(const sptr<QRImage> &img, const sptr<QRPolyScene3D> &s)
            :PolySceneCommand(s), img(img) {}

    virtual sptr<QRMemento> exec();
private:
    sptr<QRImage> img;
};


#endif //BIG3DFLUFFY_POLYSCENECOMMANDS_H
