//
// Created by kurush on 24.08.2020.
//

#include "ClimateManager.h"

ClimateManager::ClimateManager(QRMatrix<float> &hmap, QRMatrix<sptr<QRPoint3D>> &pts)
: hmap(hmap), points(pts) {
    updateMatrices(hmap, pts);
}

void ClimateManager::updateMatrices(QRMatrix<float> &hm, QRMatrix<sptr<QRPoint3D>> &pts) {
    hmap = hm;
    points = pts;

    worldStep = points[0][1]->getVector()[0] - points[0][0]->getVector()[0];
    width = worldStep * points.width();
    height = worldStep * points.height();
}

void ClimateManager::on_the_7th_day() {
    QRPolygon3D *poly;
    Vector3D v;
    size_t w, h;
    sptr<QRTexture> texture;// = QRTexturesMap[QRWATER_MATERIAL];
    int sz = polygons->getSize();
    int a, b, wdth = points.width();

    for (int k = 0; k < sz; ++k) {
        poly = (*polygons)[k].get();
        auto normal = lenNorm(poly->getNormal());
        if (isSteep(normal)) {
            texture = QRTexturesMap[QRVSTONE_MATERIAL];
        }
        else
            texture = QRTexturesMap[QRVGRASS_MATERIAL];

        /*int sz = poly->getSize();
        int32_t* indexes = poly->getPurePointIndexes();
        for (int i = 0; i < sz; ++i) {
            a = indexes[i] / wdth, b = indexes[i] % wdth;
            v = points[a][b]->getVector();
            w = getXIndex(v[0]), h = getYIndex(v[1]);

        }*/
        poly->setTexture(texture);
    }
}

bool ClimateManager::isSteep(const Vector3D &normal) {
    return fabs(normal[2]) < maxZSteepCoef;
}

size_t ClimateManager::getXIndex(float x) {
    return (x + width/2) / worldStep;
}

size_t ClimateManager::getYIndex(float y) {
    return (y + height/2) / worldStep;
}