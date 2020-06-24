//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_QRFRAME3DMEMENTO_H
#define BIG3DFLUFFY_QRFRAME3DMEMENTO_H

#include "MultipleMementos.h"
#include "../composites/interfaces/QRFrame3D.h"

class QRFrame3DMemento: public GroupMemento {
public:
    QRFrame3DMemento(sptr<QRFrame3D> obj);
    virtual void restore();

private:
    sptr<MementoAccumulator> memes;
};

#endif //BIG3DFLUFFY_QRFRAME3DMEMENTO_H
