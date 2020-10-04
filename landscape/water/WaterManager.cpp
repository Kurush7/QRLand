//
// Created by kurush on 24.08.2020.
//

#include "WaterManager.h"

WaterManager::WaterManager(QRMatrix<float> &hmap, QRMatrix<sptr<QRPoint3D>> &pts)
: hmap(hmap), points(pts), waterLevel(hmap.width(), hmap.height()) {
    updateMatrices(hmap, pts);
    waterLevel.fill(0);
}

void WaterManager::updateMatrices(QRMatrix<float> &hmap, QRMatrix<sptr<QRPoint3D>> &pts) {
    worldStep = points[0][1]->getVector()[0] - points[0][0]->getVector()[0];
    width = worldStep * points.width();
    height = worldStep * points.height();
}

void WaterManager::initErosionData() {
    sediment.resize(hmap.width(), hmap.height());
    velocity.resize(hmap.width(), hmap.height());
    flux.resize(hmap.width(), hmap.height());

    sediment.fill(0);
    velocity.fill(ZeroVector);
    flux.fill(ZeroVector);  // todo it's supposed that water initially doesn't move

    erosionReady = true;
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

    for (auto &p: changedPolygons)
        p.first->setTexture(p.second);

    changedPolygons.clear();
}

void WaterManager::updateWater() {
    if (!waterEnabled) return; // todo MUST UPDATE HMAP
    QRPolygon3D *poly;
    bool waterFlag;
    Vector3D v;
    size_t w, h;

    // todo one water-point will increase level, but no water polygons will be made. check neigbours
    int sz = polygons.getSize();
    for (int k = 0; k < sz; ++k) {
        poly = polygons[k].get();
        waterFlag = true;
        if (poly->isPointData()) {
            // todo optimize: no iterators
            for (auto p = poly->getPoints(); p; ++p) {
                v = p->get()->getVector();
                w = getXIndex(v[0]), h = getYIndex(v[1]);
                if (waterLevel[h][w] < minimalDrawWaterLevelCoef * worldStep) {
                    waterFlag = false;
                    continue;
                }

                v[2] = hmap[h][w] + waterLevel[h][w];
                p->get()->setVector(v);
            }
        }
        else {
            int a, b, wdth = points.getSize();
            int32_t* indexes = poly->getPurePointIndexes();
            int sz = poly->getSize();
            for (int i = 0; i < sz; ++i) {
                a = indexes[i] / wdth, b = indexes[i] % wdth;
                v = points[a][b]->getVector();
                w = getXIndex(v[0]), h = getYIndex(v[1]);
                if (waterLevel[h][w] < minimalDrawWaterLevelCoef * worldStep) {
                    waterFlag = false;
                    continue;
                }

                v[2] = hmap[h][w] + waterLevel[h][w];
                points[a][b]->setVector(v);
            }
        }

        auto it = changedPolygons.find(polygons[k]);
        if (waterFlag) {
            if (it == changedPolygons.end()) {
                changedPolygons[polygons[k]] = poly->getTexture();
                poly->setTexture(waterTexture);
            }
        } else {
            if (it != changedPolygons.end()) {
                it->first->setTexture(it->second);
                changedPolygons.erase(it);
            }
        }

    }
}

void WaterManager::setWaterLevel(float wl) {
    Vector3D v;
    for (size_t i = 0; i < points.height(); ++i)
        for (size_t j = 0; j < points.width(); ++j) {
            v = points[i][j]->getVector();
            waterLevel[i][j] = max(0.f, wl - v[2]);
        }

    if(waterEnabled) updateWater();
}

size_t WaterManager::getXIndex(float x) {
    return (x + width/2) / worldStep;
}

size_t WaterManager::getYIndex(float y) {
    return (y + height/2) / worldStep;
}