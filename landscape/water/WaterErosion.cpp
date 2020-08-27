//
// Created by kurush on 25.08.2020.
//

#include "WaterManager.h"

void WaterManager::erosionIteration(float dt) {
    if (!erosionReady) initErosionData();
    for (auto s: waterSources)
        s->use(dt);
    updateFlux(dt);
    updateFlux2(dt);
    erosionDeposition();
    transportSediment(dt);
    evaporation(dt);

    if (waterEnabled) updateWater();
}

void WaterManager::updateFlux(float dt) {
    // flux: 0-left, 1-right, 2-up, 3-down
    size_t w = hmap.width(), h = hmap.height();
    dt = dt*fluxPipeCapacity*gravity;
    float val, k;
    for (size_t i = 0; i < h; ++i)
        for (size_t j = 0; j < w; ++j) {
            // left
            val = hmap[i][j]+waterLevel[i][j];
            if (j == 0) flux[i][j][0] = 0;
            else flux[i][j][0] = max(0.f, flux[i][j][0] + dt*(val-hmap[i][j-1]-waterLevel[i][j-1]));
            // right
            if (j == w-1) flux[i][j][1] = 0;
            else flux[i][j][1] = max(0.f, flux[i][j][1] + dt*(val-hmap[i][j+1]-waterLevel[i][j+1]));
            // up
            if (i == 0) flux[i][j][2] = 0;
            else flux[i][j][2] = max(0.f, flux[i][j][2] + dt*(val-hmap[i-1][j]-waterLevel[i-1][j]));
            // down
            if (i == h-1) flux[i][j][3] = 0;
            else flux[i][j][3] = max(0.f, flux[i][j][3] + dt*(val-hmap[i+1][j]-waterLevel[i+1][j]));

            // scale
            if (sum(flux[i][j]) > QREPS) {
                k = min(1.f, waterLevel[i][j] * worldStep * worldStep / dt / sum(flux[i][j]));
                flux[i][j] = k * flux[i][j];
            }
        }
}

void WaterManager::updateFlux2(float dt) {
    // velocity: 0-x, 1-y
    size_t w = hmap.width(), h = hmap.height();
    float dv, dwx, dwy;
    float area = worldStep * worldStep;
    float avgWater;
    for (size_t i = 0; i < h; ++i)
        for (size_t j = 0; j < w; ++j) {
            dwx = -(flux[i][j][0] + flux[i][j][1]);
            dwy = -(flux[i][j][2] + flux[i][j][3]);
            if (j != 0) dwx += flux[i][j-1][1];
            if (j != w-1) dwx += flux[i][j+1][0];
            if (i != 0) dwy += flux[i-1][j][3];
            if (i != h-1) dwy += flux[i+1][j][2];
            dv = (dwx+dwy)*dt;
            dwx /= 2, dwy /= 2;

            avgWater = (2*waterLevel[i][j] + dv/area)/2;
            waterLevel[i][j] += dv / area;

            velocity[i][j][0] = avgWater > QREPS? dwx / worldStep / avgWater : 0;
            velocity[i][j][1] = avgWater > QREPS? dwy / worldStep / avgWater : 0;
        }
}

void WaterManager::erosionDeposition() {
    size_t w = hmap.width(), h = hmap.height();
    float c, slope;
    for (size_t i = 0; i < h; ++i)
        for (size_t j = 0; j < w; ++j) {
            slope = 0;
            if (j != 0) slope = max(slope, fabs(hmap[i][j]-hmap[i][j-1]) /
                sqrt((hmap[i][j]-hmap[i][j-1])*(hmap[i][j]-hmap[i][j-1])+ worldStep*worldStep));
            if (j != w-1) slope = max(slope, fabs(hmap[i][j]-hmap[i][j+1]) /
                                           sqrt((hmap[i][j]-hmap[i][j+1])*(hmap[i][j]-hmap[i][j+1])+ worldStep*worldStep));
            if (i != 0) slope = max(slope, fabs(hmap[i][j]-hmap[i-1][j]) /
                                           sqrt((hmap[i][j]-hmap[i-1][j])*(hmap[i][j]-hmap[i-1][j])+ worldStep*worldStep));
            if (i != h-1) slope = max(slope, fabs(hmap[i][j]-hmap[i+1][j]) /
                                           sqrt((hmap[i][j]-hmap[i+1][j])*(hmap[i][j]-hmap[i+1][j])+ worldStep*worldStep));

            c = sedimentCapacity * vectorLen2(velocity[i][j]) * slope;
            if (c > sediment[i][j]) {
                hmap[i][j] -= dissolveConstant * (c-sediment[i][j]) * worldStep;
                sediment[i][j] += dissolveConstant * (c-sediment[i][j]) * worldStep;
            }
            else {
                hmap[i][j] += depositionConstant * (sediment[i][j] - c) * worldStep;
                sediment[i][j] -= depositionConstant * (sediment[i][j] - c) * worldStep;
            };
        }
};

void WaterManager::transportSediment(float dt) {
    size_t w = hmap.width(), h = hmap.height();
    float x, y, s, len;
    size_t i1, j1;
    for (size_t i = 0; i < h; ++i)
        for (size_t j = 0; j < w; ++j) {
            x = j - velocity[i][j][0]*dt;
            y = i - velocity[i][j][1]*dt;
            i1 = round(y), j1 = round(x);
            if (fabs(x-j1) < QREPS && fabs(y-i1) < QREPS && i1>=0 && i1<h && j1>=0 && j1<w)
                sediment[i][j] = sediment[(size_t)round(y)][(size_t)round(x)];
            else {
                auto f = [x, y](float a, float b) {return sqrt((x-a)*(x-a)+(y-b)*(y-b));};
                j1 = floor(x), i1 = floor(y);
                len = 0, s = 0;

                if (i1 >= 0 && i1 < h && j1 >= 0 && j1 < w) s += sediment[i1][j1] * f(j1, i1), len+=f(j1, i1);
                j1 = ceil(x);
                if (i1 >= 0 && i1 < h && j1 >= 0 && j1 < w) s += sediment[i1][j1] * f(j1, i1), len+=f(j1, i1);
                i1 = ceil(y);
                if (i1 >= 0 && i1 < h && j1 >= 0 && j1 < w) s += sediment[i1][j1] * f(j1, i1), len+=f(j1, i1);
                j1 = floor(x);
                if (i1 >= 0 && i1 < h && j1 >= 0 && j1 < w) s += sediment[i1][j1] * f(j1, i1), len+=f(j1, i1);
                sediment[i][j] = len < QREPS? 0 : s / len;
            }
        }
}

void WaterManager::evaporation(float dt) {
    size_t w = hmap.width(), h = hmap.height();
    dt = 1 - dt * evaporationCoef;
    for (size_t i = 0; i < h; ++i)
        for (size_t j = 0; j < w; ++j)
            waterLevel[i][j] *= dt;
}