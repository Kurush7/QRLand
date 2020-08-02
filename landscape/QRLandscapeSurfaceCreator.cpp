//
// Created by kurush on 01.08.2020.
//

#include "QRLandscapeSurfaceCreator.h"
#include <cmath>

uptr<QRLandscapeSurface> RandomHMapLandscapeSurfaceCreator::create() {
    QRMatrix<double> m(width, height);
    double x = -(width+0.)/2.*step, y = -(height+0.)/2.*step;
    for(size_t i = 0; i < height; ++i) {
        x = -(width+0.)/2.*step;
        for (size_t j = 0; j < width; ++j) {
            m[i][j] = exp(-(x * x + y * y) / 8) * (sin(x * x) + cos(y * y));
            cout << x << ' ' << y << ' ' << m[i][j] << '\n';
            x += step;
        }
        y += step;
    }

    return uptr<QRLandscapeSurface>(new QRLandscapeSurface(m, step));
}