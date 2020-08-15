//
// Created by kurush on 11.08.2020.
//

#include "RoamLandscapeCreator.h"

uptr<QRPolyModel3D> RoamLandscapeCreator::create() {
    QRMatrix<sptr<QRPoint3D>> m(width, height);
    double x, y = -(height+0.)/2.*step;
    for(size_t i = 0; i < height; ++i) {
        x = -(width+0.)/2.*step;
        for (size_t j = 0; j < width; ++j) {
            double z = exp(-(x * x + y * y) / 8) * (sin(x * x) + cos(y * y));
            m[i][j] = sptr<QRPoint3D>(new Point3D((j-(double)(width)/2)*step,
                    (i-(double)(height)/2)*step, z));
            x += step;
        }
        y += step;
    }

    return uptr<QRPolyModel3D>(new RoamLandscape(m));
}