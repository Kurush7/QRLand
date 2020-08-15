//
// Created by kurush on 27.06.2020.
//

#ifndef BIG3DFLUFFY_QRPOLYMODELMEMENTO_H
#define BIG3DFLUFFY_QRPOLYMODELMEMENTO_H

#include "QRMemento.h"
#include "MultipleMementos.h"
#include "../composites/interfaces/QRPolyModel3D.h"

class QRPolyModelMemento: public QRMemento {
public:
    QRPolyModelMemento(sptr<QRPolyModel3D> obj);
    virtual void restore();

private:
    wptr<QRPolyModel3D> object;
    QRVector<sptr<QRPoint3D>> points;
    QRVector<sptr<QRPolygon3D>> polygons;
    sptr<MementoAccumulator> memesAccum;
};

#endif //BIG3DFLUFFY_QRPOLYMODELMEMENTO_H
