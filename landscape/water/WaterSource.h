//
// Created by kurush on 25.08.2020.
//

#ifndef BIG3DFLUFFY_WATERSOURCE_H
#define BIG3DFLUFFY_WATERSOURCE_H

#include <random>
#include "containers/QRContainers.h"
#include "math/QRMath.h"
#include "objects/objects.h"
#include "QRConstants.h"

// todo add climate here

class WaterSource {
public:
    WaterSource(QRMatrix<float> &waterLevel): waterLevel(waterLevel) {}
    virtual void use(float dt) = 0;
    virtual void scaleGrid(QRMatrix<float> &newWL) {waterLevel = newWL;}
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

    virtual void scaleGrid(QRMatrix<float> &newWL) {
        WaterSource::scaleGrid(newWL);
        width = waterLevel.width(), height = waterLevel.height();
        dist_w = uniform_int_distribution<size_t>(0, waterLevel.width()-1);
        dist_h = uniform_int_distribution<size_t>(0, waterLevel.height()-1);
    }

    virtual void use(float dt);

private:
    float dropIntensity;
    int dropCnt, dropRad;
    size_t width, height;
    std::default_random_engine generator = std::default_random_engine();
    uniform_int_distribution<size_t> dist_w, dist_h;
};



class RiverSource: public WaterSource {
public:
    // radius is set in grid's amount (rad=2: two adjacent values in a grid)
    RiverSource(QRMatrix<float> &waterLevel, float riverIntensity, size_t pos_x, size_t pos_y):
            WaterSource(waterLevel), riverIntensity(riverIntensity){
        x = min(pos_x, waterLevel.width());
        y = min(pos_y, waterLevel.height());
    }

    virtual void scaleGrid(QRMatrix<float> &newWL) {
        WaterSource::scaleGrid(newWL);
        x *= 2;
        y *= 2;
    }

    virtual void use(float dt) {
        waterLevel[y][x] += riverIntensity;
    }

private:
    float riverIntensity;
    size_t x, y;
};


#endif //BIG3DFLUFFY_WATERSOURCE_H
