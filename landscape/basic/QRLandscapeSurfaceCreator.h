//
// Created by kurush on 01.08.2020.
//

#ifndef BIG3DFLUFFY_QRLANDSCAPESURFACECREATOR_H
#define BIG3DFLUFFY_QRLANDSCAPESURFACECREATOR_H

#include "QRLandscapeSurface.h"

class QRLandscapeSurfaceCreator {
public:
    virtual uptr<QRLandscapeSurface> create() = 0;
};

class RandomHMapLandscapeSurfaceCreator: public QRLandscapeSurfaceCreator {
public:
    RandomHMapLandscapeSurfaceCreator(size_t width, size_t height, double step=1) {
        setParams(width, height, step);
    }
    // todo random seed

    void setParams(size_t w, size_t h, double _step) {
        width = w, height = h;
        step = _step;
    }

    virtual uptr<QRLandscapeSurface> create();

private:
    size_t width, height;
    double step;
};

#endif //BIG3DFLUFFY_QRLANDSCAPESURFACECREATOR_H
