//
// Created by kurush on 28.06.2020.
//

#ifndef BIG3DFLUFFY_QRCANVASVIEWER_H
#define BIG3DFLUFFY_QRCANVASVIEWER_H

#include <QGraphicsView>
#include <algorithm>

#include "../globalDefines.h"
#include "2DStructures.h"

const double zoomRatio = 1.1;
const double zoomSensivity = 4;
const double moveSensivity = 32;

class QRCanvasViewer {
public:
    QRCanvasViewer(sptr<QImage> img = nullptr): src(img), k(1), dx(0), dy(0) {}
    void drawOn(sptr<QImage> img);

    void zoom(Point, double);
    void move(double, double);

    void reset();
private:
    void validateD();
    void validateK();

    sptr<QImage> src;
    double dx, dy, k;
};

#endif //BIG3DFLUFFY_QRCANVASVIEWER_H
