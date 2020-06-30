//
// Created by kurush on 27.06.2020.
//

#include "PolyModel.h"

void PolyModel::setValues(PointIterator pt, PolygonIterator pg) {
    points = pt,
    polygons = pg;
    defineRadius();
}

void PolyModel::defineRadius() {
    radius = 0;
    for (auto p: points)
        radius = max(radius, vectorLen(p->getVector()));
}