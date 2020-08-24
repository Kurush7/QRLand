//
// Created by kurush on 24.08.2020.
//

#include "WaterManager.h"

WaterManager::WaterManager(QRMatrix<float> &hmap, QRMatrix<sptr<QRPoint3D>> &pts)
: hmap(hmap), points(pts), waterLevel(hmap.width(), hmap.height()) {
    worldStep = points[0][1]->getVector()[0] - points[0][0]->getVector()[0];
    width = worldStep * points.width();
    height = worldStep * points.height();

    waterLevel.fill(0);
}

void WaterManager::resetWater() {
    // reset heightMap
    Vector3D v;
    for (size_t i = 0; i < points.height(); ++i)
        for (size_t j = 0; j < points.width(); ++j) {
            v = points[i][j]->getVector();
            v[2] -= waterLevel[i][j];
            points[i][j]->setVector(v);
        }

    for (size_t i = 0; i < changedTextures.getSize(); ++i)
        *(changedTextures[i]) = bottomTextureValues[i];

    changedTextures.clear();    // no need....
    bottomTextureValues.clear();
}

void WaterManager::updateWater() {
    QRPolygon3D* poly;
    bool waterFlag;
    Vector3D v;
    size_t w, h;

    changedTextures.clear();
    bottomTextureValues.clear();
    // todo one water-point will increase level, but no water polygons will be made. check neigbours
    for (size_t k = 0; k < polygons.getSize(); ++k) {
        poly = polygons[k].get();
        waterFlag = true;
        for (auto p = poly->getPoints(); p; ++p) {
            v = p->get()->getVector();
            w = getXIndex(v[0]), h = getYIndex(v[1]);
            cout << w << ' ' << h << " -> " << v[2] << ' ' << hmap[h][w] << ' ';
            if (waterLevel[h][w] < QREPS) {
                waterFlag = false;
                cout << '\n';
                continue;
            }
            if (fabs(v[2] - hmap[h][w]) < QREPS) {
                cout << '#';
                v[2] += waterLevel[h][w];
                p->get()->setVector(v);
            }
            cout << '\n';
        }

        if (waterFlag) {
            changedTextures.push_back(&(poly->getTextureUnsafe()));
            bottomTextureValues.push_back(poly->getTexture());
            poly->setTexture(waterTexture);
            cout << "***";
        }
        cout << '\n';
    }
}

void WaterManager::setWaterLevel(float wl) {
    Vector3D v;
    for (size_t i = 0; i < points.height(); ++i)
        for (size_t j = 0; j < points.width(); ++j) {
            v = points[i][j]->getVector();
            waterLevel[i][j] = max(0.f, wl - v[2]);
            cout << waterLevel[i][j] <<  ' ' << v[2] << '\n';
        }

    if(waterEnabled) updateWater();
}

size_t WaterManager::getXIndex(float x) {
    return (x + width/2) / worldStep;
}

size_t WaterManager::getYIndex(float y) {
    return (y + height/2) / worldStep;
}