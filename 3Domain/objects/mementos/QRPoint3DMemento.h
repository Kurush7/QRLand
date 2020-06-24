//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_QRPOINT3DMEMENTO_H
#define BIG3DFLUFFY_QRPOINT3DMEMENTO_H

#include <memory>

#include "QRMemento.h"
#include "components/interfaces/QRPoint3D.h"

class QRPoint3DMemento: public QRMemento {
public:
    QRPoint3DMemento(sptr<QRPoint3D>);
    virtual void restore();

private:
    wptr<QRPoint3D> object;
    Vector3D point, bind;
    QRPointStyle style;
};


#endif //BIG3DFLUFFY_QRPOINT3DMEMENTO_H
