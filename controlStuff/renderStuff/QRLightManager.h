//
// Created by kurush on 03.07.2020.
//

#ifndef BIG3DFLUFFY_QRLIGHTMANAGER_H
#define BIG3DFLUFFY_QRLIGHTMANAGER_H

#include "3Domain/objects/lights/light.h"
#include "3Domain/textures/textures.h"
#include "../QRConstants.h"

class QRLightManager {
public:
    QRLightManager() {
        lights = new QRLight*[MAX_LIGHT_CNT];
        for (int i = 0; i < MAX_LIGHT_CNT; ++i)
            lights[i] = nullptr;
    }

    void addLight(const sptr<QRLight> &light) {lights[size++] = light.get();} // todo check for max_size
    // todo delete light

    void lightenColor(const Vector3D &normal, QRColor &c) {
        Vector3D intens = {0,0,0,0};
        for (int i = 0; i < size; ++i)
            intens += lights[i]->getIntensity({0, 0, 0}, normal); // todo no pos here

        c.r = intens[0] * c.r;
        c.g = intens[1] * c.g;
        c.b = intens[2] * c.b;
    }

private:
    QRLight** lights;
    int size = 0;
};


#endif //BIG3DFLUFFY_QRLIGHTMANAGER_H
