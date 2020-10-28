//
// Created by kurush on 24.08.2020.
//

#include "WaterManager.h"

WaterManager::WaterManager(QRMatrix<float> &hmap, QRMatrix<sptr<QRPoint3D>> &pts)
: hmap(hmap), points(pts), waterLevel(hmap.width(), hmap.height()) {
    updateMatrices(hmap, pts);
    waterLevel.fill(0);
}

void WaterManager::updateMatrices(QRMatrix<float> &hm, QRMatrix<sptr<QRPoint3D>> &pts) {
    hmap = hm;
    points = pts;

    float w = hm.width(), h = hm.height();
    auto newWL = QRMatrix<float>(w, h);
    for (int i = 0, oldi=0; i < h; i += 2, oldi++)
        for (int j = 0, oldj=0; j < w; j+= 2, oldj++)
            newWL[i][j] = waterLevel[oldi][oldj];
    float tmp, cnt;

    for (int i = 1; i < h; i += 2)
        for (int j = 1; j < w; j += 2)
            newWL[i][j] = (newWL[i-1][j - 1] + newWL[i-1][j + 1] + newWL[i+1][j-1] + newWL[i+1][j+1]) / 4;

    for (int i = 0; i < h; i++)
        for (int j = i%2? 0:1; j < w; j+=2) {
            tmp = 0, cnt=0;
            if (j > 0) tmp += newWL[i][j - 1], cnt++;
            if (j < w-1) tmp += newWL[i][j + 1], cnt++;
            if (i > 0) tmp += newWL[i-1][j], cnt++;
            if (i < h-1) tmp += newWL[i+1][j], cnt++;
            newWL[i][j] = tmp / cnt;
        }
    waterLevel = newWL;


    worldStep = points[0][1]->getVector()[0] - points[0][0]->getVector()[0];
    width = worldStep * points.width();
    height = worldStep * points.height();
    erosionReady = false;

    for (auto &s: waterSources)
        s->scaleGrid(waterLevel);
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

void WaterManager::setWaterMatrix(QRMatrix<float>&m) {
    if (m.width() != waterLevel.width() || m.height() != waterLevel.height()) {
        cerr << "updating hmap: wrong sizes";
        return;
    }
    for (size_t i = 0; i < m.height(); ++i)
        for (size_t j = 0; j < m.width(); ++j)
            waterLevel[i][j] = m[i][j];

    updateWater();
}

void WaterManager::updateWater() {
    if (!waterEnabled) return; // todo MUST UPDATE HMAP
    QRPolygon3D *poly;
    bool waterFlag;
    Vector3D v;
    size_t w, h;

    // todo one water-point will increase level, but no water polygons will be made. check neigbours
    int sz = polygons->getSize();
    int a, b, wdth = points.width();
    for (int k = 0; k < sz; ++k) {
        poly = (*polygons)[k].get();
        waterFlag = true;

        int32_t* indexes = poly->getPurePointIndexes();
        int sz = poly->getSize();
        for (int i = 0; i < sz; ++i) {
            a = indexes[i] / wdth, b = indexes[i] % wdth;
            v = points[a][b]->getVector();
            w = getXIndex(v[0]), h = getYIndex(v[1]);

            v[2] = hmap[h][w] + waterLevel[h][w];
            points[a][b]->setVector(v);
            if (waterLevel[h][w] < minimalDrawWaterLevelCoef * worldStep) waterFlag = false;
        }
        poly->updateNormal();

        auto it = changedPolygons.find((*polygons)[k]);
        if (waterFlag) {
            if (it == changedPolygons.end())
                changedPolygons[(*polygons)[k]] = poly->getTexture();

            poly->setTexture(waterTexture);
        }
        else {
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