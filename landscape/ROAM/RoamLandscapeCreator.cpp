//
// Created by kurush on 11.08.2020.
//

#include "RoamLandscapeCreator.h"

sptr<QRPolyModel3D> RoamLandscapeCreator::create() {
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

    return sptr<QRPolyModel3D>(new RoamLandscape(m));
}

void RoamLandscapeCreator::updateHeightMap(const QRMatrix<double> &hMap) {
    if (hMap.width() != m.width() || hMap.height() != m.height())
        throw QRBadParamException(__FILE__, __LINE__, __TIME__,
                "heightmap size not equal to point-matrix size");
    Vector3D v;
    for(size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            v = m[i][j]->getVector();
            v[3] = hMap[i][j];
            m[i][j]->setVector(v);
        }
    }
}