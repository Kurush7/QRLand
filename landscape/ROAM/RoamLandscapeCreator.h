//
// Created by kurush on 11.08.2020.
//

#ifndef BIG3DFLUFFY_ROAMLANDSCAPECREATOR_H
#define BIG3DFLUFFY_ROAMLANDSCAPECREATOR_H

#include "RoamLandscape.h"

class RoamLandscapeCreator {
public:
    RoamLandscapeCreator(size_t width, size_t height, double step = 1): m(width, height) {
        setParams(width, height, step);
    }

    void setParams(size_t w, size_t h, double _step) {
        width = w, height = h;
        step = _step;
    }

    virtual sptr<QRPolyModel3D> create();
    virtual void updateHeightMap(const QRMatrix<double> &hMap);

private:
    QRMatrix<sptr<QRPoint3D>> m;
    size_t width, height;
    double step;
};

#endif //BIG3DFLUFFY_ROAMLANDSCAPECREATOR_H
