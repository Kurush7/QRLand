//
// Created by kurush on 24.08.2020.
//

#ifndef BIG3DFLUFFY_WATERMANAGER_H
#define BIG3DFLUFFY_WATERMANAGER_H

#include "containers/QRContainers.h"
#include "math/QRMath.h"
#include "objects/objects.h"
#include "basic/QRLandscapeTexture.h"

// todo some hardcoding here about "points are in range [-width/2, width/2][-height/2, height/2]"
// todo define worldstep etc. each time on updating for independence
class WaterManager {
public:
    WaterManager(QRMatrix<float> &hmap, QRMatrix<sptr<QRPoint3D>> &pts);

    // todo update hmap etc. here

    void enableWater() {waterEnabled = true; updateWater();} //todo
    void disableWater() {waterEnabled = false; resetWater();} //todo
    bool isWaterEnabled() {return waterEnabled;}
    void setWaterStatus(bool x) {x? enableWater() : disableWater(); }

    void setWaterLevel(float wl);

    void setPolygons(const QRVectorIterator<sptr<QRPolygon3D>> &polys) {polygons = polys;}

private:
    float worldStep, width, height;
    QRMatrix<float> &hmap;
    QRMatrix<sptr<QRPoint3D>> &points;
    QRVector<sptr<QRPolygon3D>> polygons;   // todo link?

    size_t getXIndex(float x);
    size_t getYIndex(float y);

    QRMatrix<float> waterLevel;

    QRVector<sptr<QRTexture>*> changedTextures;
    QRVector<sptr<QRTexture>> bottomTextureValues;
    sptr<QRTexture> waterTexture = QRTexturesMap[QRWATER_MATERIAL];

    bool waterEnabled = true;

    void resetWater();
    void updateWater();
};


#endif //BIG3DFLUFFY_WATERMANAGER_H
