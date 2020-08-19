//
// Created by kurush on 19.08.2020.
//

#include "PlateManager.h"

QRVector<Vector3D> PlateManager::definePoints(int cnt) {
    uniform_real_distribution<double> gx(0, w);
    uniform_real_distribution<double> gy(0, h);

    float x, y;
    QRVector<Vector3D> pts;
    for (int i = 0; i < cnt; ++i) {
        x = gx(generator), y = gy(generator);
        pts.push_back({x,y,0});
    }

    return pts;
}


void PlateManager::buildPlates(QRVector<Vector3D> points) {
    plates = buildVoronoiDiagramOnRect(0,w,0,h, points);
    for (int i = 0; i < plates.getSize(); ++i)
        plates[i]->setColor(plateBorderColor);
}