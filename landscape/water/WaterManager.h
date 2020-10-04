//
// Created by kurush on 24.08.2020.
//

#ifndef BIG3DFLUFFY_WATERMANAGER_H
#define BIG3DFLUFFY_WATERMANAGER_H

#include "containers/QRContainers.h"
#include "math/QRMath.h"
#include "objects/objects.h"
#include "basic/QRLandscapeTexture.h"

#include "WaterConfig.h"
#include "WaterSource.h"


// todo some hardcoding here about "points are in range [-width/2, width/2][-height/2, height/2]"
// todo define worldstep etc. each time on updating for independence
class WaterManager {
public:
    WaterManager(QRMatrix<float> &hmap, QRMatrix<sptr<QRPoint3D>> &pts);
    void setPolygons(const QRVectorIterator<sptr<QRPolygon3D>> &polys) {polygons = polys;}
    void updateMatrices(QRMatrix<float> &hmap, QRMatrix<sptr<QRPoint3D>> &pts);
    // todo update hmap etc. here

    void addRiverSource(size_t x, size_t y, float intensity=1) {
        waterSources.push_back(sptr<WaterSource>(new RiverSource(waterLevel,
                                                                 intensity*worldStep*riverIntencityCoef,
                                                                 x, y)));
    }

    void addRainSource(float dropCnt = rainDropCnt, float intensity=1) {
        waterSources.push_back(sptr<WaterSource>(new RainWaterSource(waterLevel,
                                                                     intensity*worldStep*rainDropIntencityCoef,
                                                                     dropCnt)));
    }

    void enableWater() {
        if (waterEnabled) return;
        waterEnabled = true;
        updateWater();
    }
    void disableWater() {
        if (!waterEnabled) return;
        waterEnabled = false; resetWater();
    }
    bool isWaterEnabled() {return waterEnabled;}
    void setWaterStatus(bool x) {x? enableWater() : disableWater(); }

    void setWaterLevel(float wl);
    void erosionIteration(float dt = defaultErosionDT);

    void updateWater();
private:
    bool waterEnabled = true;
    float worldStep, width, height;

    QRMatrix<sptr<QRPoint3D>> &points;
    QRVector<sptr<QRPolygon3D>> polygons;   // todo link?

    QRMatrix<float> &hmap;
    QRMatrix<float> waterLevel;
    QRMatrix<float> sediment;
    QRMatrix<Vector3D> flux, velocity;  // todo velocity sufficient to be pair

    std::map<sptr<QRPolygon3D>, sptr<QRTexture>> changedPolygons;
    sptr<QRTexture> waterTexture = QRTexturesMap[QRWATER_MATERIAL];

    QRVector<sptr<WaterSource>> waterSources;


    size_t getXIndex(float x);
    size_t getYIndex(float y);

    bool erosionReady = false;
    void initErosionData();
    void resetWater();

    void updateFlux(float dt);
    void updateFlux2(float dt);
    void erosionDeposition();
    void transportSediment(float dt);
    void evaporation(float dt);
};


#endif //BIG3DFLUFFY_WATERMANAGER_H
