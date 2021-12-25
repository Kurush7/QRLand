//
// Created by kurush on 27.06.2020.
//

#include "QRPolyModelMemento.h"

QRPolyModelMemento::QRPolyModelMemento(sptr<QRPolyModel3D> obj): memesAccum(new MementoAccumulator()) {
    object = obj;
    for (auto x = obj->getPoints(); x; ++x)
        memesAccum->add((*x)->save()), points.push_back(*x);
    for (auto x = obj->getPolygons(); x; ++x)
        memesAccum->add((*x)->save()), polygons.push_back(*x);
}
void QRPolyModelMemento::restore() {
    if (object.expired())
        throw QRBadPointerException(__FILE__, __LINE__, __TIME__, "Failed to create memento!");
    object.lock()->setValues(points.begin(), polygons.begin());
    memesAccum->restore();
}