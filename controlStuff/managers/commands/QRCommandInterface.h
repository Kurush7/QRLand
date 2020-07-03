//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_QRCOMMANDINTERFACE_H
#define BIG3DFLUFFY_QRCOMMANDINTERFACE_H


#include "objects/objects.h"
#include "objects/mementos/MultipleMementos.h"
#include "../../../globalDefines.h"

class QRCommand {
public:
    virtual sptr<QRMemento> exec() = 0;
};

// todo legacy
class SceneCommand: public QRCommand {
public:
    SceneCommand(sptr<QRScene3D> s): scene(s) {}
protected:
    sptr<QRScene3D> scene;
};

class PolySceneCommand: public QRCommand {
public:
    PolySceneCommand(const sptr<QRPolyScene3D> &s): scene(s) {}
protected:
    sptr<QRPolyScene3D> scene;
};

#endif //BIG3DFLUFFY_QRCOMMANDINTERFACE_H