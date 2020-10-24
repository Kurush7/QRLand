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
        x = gx(default_generator), y = gy(default_generator);
        pts.push_back({x,y,0});
    }

    return pts;
}


void PlateManager::buildPlates(QRVector<Vector3D> points) {
    plates = buildVoronoiDiagramOnRect(0,w-1,0,h-1, points);
    for (int i = 0; i < plates.getSize(); ++i)
        plates[i]->setColor(QRColor("green"));  //todo COLOR HARDCODE

    uniform_real_distribution<double> gd(0, 1);
    uniform_real_distribution<double> gdir(-1, 1);
    for (int i = 0; i < plates.getSize(); ++i) {
        double x = gdir(default_generator), y = gdir(default_generator);
        Vector3D vec(x,y,0,0);
        vec = lenNorm(vec);
        vec = vec * gd(default_generator);
        moveVectors.push_back(vec);
    }

}