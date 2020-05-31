//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_PAINTER_H
#define BIG3DFLUFFY_PAINTER_H

#include "objects/BaseObject.h"

class Painter {
public:
    virtual void drawPoint(double x, double y, QRPointStyle) = 0;
    virtual void drawEdge(double x1, double y1, double x2, double y2, QREdgeStyle) = 0;
};


#endif //BIG3DFLUFFY_PAINTER_H
