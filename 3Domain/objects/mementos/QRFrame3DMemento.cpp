//
// Created by kurush on 24.06.2020.
//

#include "QRFrame3DMemento.h"

QRFrame3DMemento::QRFrame3DMemento(sptr<QRFrame3D> obj): GroupMemento(obj), memes(new MementoAccumulator()) {
    for (auto x: obj->getObjects())
        memes->add(x->save());
}
void QRFrame3DMemento::restore() {
    GroupMemento::restore();
    memes->restore();
}