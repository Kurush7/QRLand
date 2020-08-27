//
// Created by kurush on 25.08.2020.
//

#ifndef BIG3DFLUFFY_WATERSOURCE_H
#define BIG3DFLUFFY_WATERSOURCE_H

#include <random>
#include "containers/QRContainers.h"
#include "math/QRMath.h"
#include "objects/objects.h"
#include "WaterConfig.h"

// todo add climate here

class WaterSource {
public:
    WaterSource(QRMatrix<float> &waterLevel): waterLevel(waterLevel) {}
    virtual void use(float dt) = 0;
protected:
    QRMatrix<float> &waterLevel;
};

class RainWaterSource: public WaterSource {
public:
    // radius is set in grid's amount (rad=2: two adjacent values in a grid)
    RainWaterSource(QRMatrix<float> &waterLevel, float dropIntensity,
            int dropCnt = rainDropCnt, int dropRad = rainDropRadius):
            WaterSource(waterLevel), dropIntensity(dropIntensity),
            dropCnt(dropCnt), dropRad(dropRad) {
        width = waterLevel.width(), height = waterLevel.height();
        dist_w = uniform_int_distribution<size_t>(0, waterLevel.width()-1);
        dist_h = uniform_int_distribution<size_t>(0, waterLevel.height()-1);
    }

    virtual void use(float dt) {
        size_t w, h;
        for(int k = 0; k < dropCnt; ++k) {
            w = dist_w(generator);
            h = dist_h(generator);
            for (int i = -dropRad+1; i < dropRad; ++i)
                for (int j = -dropRad+1; j < dropRad; ++j)
                    if (w+i < width && w+i >= 0 && h+j<height && h+j >= 0)
                        waterLevel[h+j][w+i] += dropIntensity;
        }
    }

private:
    float dropIntensity;
    int dropCnt, dropRad;
    size_t width, height;
    std::default_random_engine generator = std::default_random_engine();
    uniform_int_distribution<size_t> dist_w, dist_h;
};

#endif //BIG3DFLUFFY_WATERSOURCE_H
