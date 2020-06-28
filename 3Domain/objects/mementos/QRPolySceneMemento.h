//
// Created by kurush on 28.06.2020.
//

#ifndef BIG3DFLUFFY_QRPOLYSCENEMEMENTO_H
#define BIG3DFLUFFY_QRPOLYSCENEMEMENTO_H

#include "QRMemento.h"
#include "MultipleMementos.h"
#include "../composites/interfaces/QRPolyScene3D.h"

class QRPolySceneMemento: public QRMemento {
public:
    explicit QRPolySceneMemento(sptr<QRPolyScene3D> scene);
    virtual void restore();
private:
    wptr<QRPolyScene3D> scene;
    sptr<QRCamera3D> camera;
    QRVector<sptr<QRPolyModel3D>> models;
    sptr<MementoAccumulator> memes;
};


#endif //BIG3DFLUFFY_QRPOLYSCENEMEMENTO_H
