//
// Created by kurush on 11.08.2020.
//

#ifndef BIG3DFLUFFY_ROAMLANDSCAPECREATOR_H
#define BIG3DFLUFFY_ROAMLANDSCAPECREATOR_H

#include "RoamLandscape.h"

class RoamLandscapeCreator {
public:
    RoamLandscapeCreator(size_t width, size_t height, double step = 1) {
        setParams(width, height, step);
    }

    void setParams(size_t w, size_t h, double _step) {
        width = w, height = h;
        step = _step;
    }

    virtual uptr<QRPolyModel3D> create();

private:
    size_t width, height;
    double step;
};

#endif //BIG3DFLUFFY_ROAMLANDSCAPECREATOR_H
