//
// Created by kurush on 27.06.2020.
//

#include "QRPolyModel3D.h"
#include "../../mementos/QRPolyModelMemento.h"

uptr<QRMemento> QRPolyModel3D::save() {return uptr<QRMemento>(new QRPolyModelMemento(p_model));}

bool QRPolyModel3D::operator==(const QRPolyModel3D &b) const {
    // todo check shift in order

    auto x = getPoints(), y = b.getPoints();
    for (; x && y; ++x, ++y)
        if (*x != *y) return false;
    if (!x || !y) return false;

    auto p = getPolygons(), q = b.getPolygons();
    for (; p && q; ++p, ++q)
        if (*p != *q) return false;
    if (!p || !q) return false;

    return true;
}