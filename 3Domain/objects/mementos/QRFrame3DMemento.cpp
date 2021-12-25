//
// Created by kurush on 24.06.2020.
//

#include "QRFrame3DMemento.h"

QRFrame3DMemento::QRFrame3DMemento(sptr<QRFrame3D> obj): memesAccum(new MementoAccumulator()) {
    object = obj;
    for (auto x: obj->getObjects())
        memesAccum->add(x->save());
}
void QRFrame3DMemento::restore() {
    if (object.expired())
        throw QRBadPointerException(__FILE__, __LINE__, __TIME__, "Failed to create memento!");
    object.lock()->setObjects(memes);
    memesAccum->restore();
}