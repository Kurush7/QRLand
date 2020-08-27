//
// Created by kurush on 01.08.2020.
//

#include "QRLandscapeSurfaceCreator.h"
#include <cmath>

uptr<QRLandscapeSurface> RandomHMapLandscapeSurfaceCreator::create() {
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

    return uptr<QRLandscapeSurface>(new QRLandscapeSurface(m));
}