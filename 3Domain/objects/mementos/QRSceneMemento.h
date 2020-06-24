//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_QRSCENEMEMENTO_H
#define BIG3DFLUFFY_QRSCENEMEMENTO_H

#include "QRMemento.h"
#include "../composites/interfaces/QRScene3D.h"

class QRSceneMemento: public QRMemento {
public:
    explicit QRSceneMemento(sptr<QRScene3D> scene);
    virtual void restore();
private:
    wptr<QRScene3D> scene;   // todo camera is keeped, but not restored, since there is no setCamera method
    wptr<QRCamera3D> camera;
    QRVector<sptr<QRObject>> objects;
};


#endif //BIG3DFLUFFY_QRSCENEMEMENTO_H
