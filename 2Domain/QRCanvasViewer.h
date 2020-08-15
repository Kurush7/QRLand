//
// Created by kurush on 28.06.2020.
//

#ifndef BIG3DFLUFFY_QRCANVASVIEWER_H
#define BIG3DFLUFFY_QRCANVASVIEWER_H

#include <QImage>
#include <algorithm>

#include "../globalDefines.h"
#include "2DStructures.h"

const float zoomRatio = 1.1;
const float zoomSensivity = 4;
const float moveSensivity = 32;

class QRCanvasViewer {
public:
    QRCanvasViewer(sptr<QImage> img = nullptr): src(img), k(1), dx(0), dy(0) {}
    void drawOn(sptr<QImage> img);

    void zoom(Point, float);
    void move(float, float);

    void reset();
private:
    void validateD();
    void validateK();

    sptr<QImage> src;
    float dx, dy, k;
};

#endif //BIG3DFLUFFY_QRCANVASVIEWER_H
