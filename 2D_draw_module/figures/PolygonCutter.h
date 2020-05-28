//
// Created by kurush on 14.05.2020.
//

#ifndef KG_POLYGONCUTTER_H
#define KG_POLYGONCUTTER_H

#include "Figures.h"

struct Debugger {
    Polygon Pdebug = Polygon(std::vector<Point>());
    int debugIter=0;
};

std::vector<Polygon> dividePoly(const Polygon &);

bool cutPolygon(const Polygon &cutter, const Polygon &poly, Polygon &res);
bool cutPolygonDebug(const Polygon &cutter, const Polygon &polygon, Polygon &res, bool init=false);


#endif //KG_POLYGONCUTTER_H
