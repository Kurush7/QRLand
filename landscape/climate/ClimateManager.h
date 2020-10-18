//
// Created by kurush on 24.08.2020.
//

#ifndef BIG3DFLUFFY_CLIMATEMANAGER_H
#define BIG3DFLUFFY_CLIMATEMANAGER_H

#include "containers/QRContainers.h"
#include "math/QRMath.h"
#include "objects/objects.h"
#include "basic/QRLandscapeTexture.h"

#include "QRConstants.h"


class ClimateManager {
public:
    ClimateManager(QRMatrix<float> &hmap, QRMatrix<sptr<QRPoint3D>> &pts);
    void setPolygons(QRVector<sptr<QRPolygon3D>> *polys) {polygons = polys;}
    void updateMatrices(QRMatrix<float> &hmap, QRMatrix<sptr<QRPoint3D>> &pts);

    void on_the_7th_day();

private:
    float worldStep, width, height;

    QRMatrix<sptr<QRPoint3D>> &points;
    QRVector<sptr<QRPolygon3D>> *polygons;

    QRMatrix<float> &hmap;

    bool isSteep(const Vector3D &normal);

    size_t getXIndex(float x);
    size_t getYIndex(float y);
};


#endif //BIG3DFLUFFY_CLIMATEMANAGER_H
