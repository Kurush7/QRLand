//
// Created by kurush on 27.06.2020.
//

#ifndef BIG3DFLUFFY_QRPOLYGON3DMEMENTO_H
#define BIG3DFLUFFY_QRPOLYGON3DMEMENTO_H

#include "QRMemento.h"
#include "objects/components/interfaces/QRPolygon3D.h"

class QRPolygon3DMemento: public QRMemento {
public:
    QRPolygon3DMemento(sptr<QRPolygon3D>);
    virtual void restore();

private:
    wptr<QRPolygon3D> object;
    QRVector<sptr<QRPoint3D>> points;
    Vector3D plane; // todo just in case. really it's not needed, for a polygon now is not supporting any changes
    sptr<QRTexture> texture;
};


#endif //BIG3DFLUFFY_QRPOLYGON3DMEMENTO_H
