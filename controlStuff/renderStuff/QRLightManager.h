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

    ~QRLightManager() {
        delete[] lights;
        delete[] shadeZBuf;
    }

    QRLight* getLight(int pos) {return lights[pos];}

    void addLight(const sptr<QRLight> &light) {lights[size++] = light.get();} // todo check for max_size
    // todo delete light

    void lightenColor(const Vector3D &pos, const Vector3D &normal, const QRColor &c, QRColor &res) {
        Vector3D intens = ZeroVector;
        // todo optimize: precompute all ambiences

        if (shading) {
            Vector3D v = reProjectMatrix * pos;
            int x = round(v[0]), y = round(v[1]);
            //cout << shadeZBuf[y*shadeW + x] << '\n';
            if (shadeZBuf[y*shadeW + x] < v[2])
                intens = lights[lightShaded]->getAmbient();
            else
                for (int i = 0; i < size; ++i)
                    intens += lights[i]->getIntensity(pos, normal);
        }
        else
            for (int i = 0; i < size; ++i)
                intens += lights[i]->getIntensity(pos, normal);

        res.r = intens[0] * c.r;
        res.g = intens[1] * c.g;
        res.b = intens[2] * c.b;
        // todo alpha here....)))
    }

    void transformLightsPosition(const QRTransformer3D *trans) {
        for (int i = 0; i < size; ++i)
            lights[i]->transform(trans);
    }
    void resetLightsPosition() {
        for (int i = 0; i < size; ++i)
            lights[i]->resetPosition();
    }

    // shade-management
    void setShadesZBuf(float *zbuf, int w, int h, int lightUsed) {
        lightShaded = lightUsed;
        if (w*h>maxShadeSize) {
            if (shadeZBuf) delete shadeZBuf;
            maxShadeSize = w*h;
            shadeZBuf = new float[maxShadeSize];
        }

        shadeW=w, shadeH=h;
        for (int i = 0; i < w*h; ++i) {
            shadeZBuf[i] = zbuf[i];
        }
    }

    void setTransformPath(const Matrix3D &camProjToGlobal, const Matrix3D &globalToLightProj) {
        //Matrix3D tmp = globalToLightProj * camProjToGlobal;
        //reProjectMatrix = QuickMatrix(tmp);
        reProjectMatrix = globalToLightProj * camProjToGlobal;
    }

    void useShades(bool x=true) {shading = x;}


private:
    QRLight** lights;
    int size = 0;

    float *shadeZBuf = nullptr;
    int shadeW=0, shadeH=0, maxShadeSize=0;
    int lightShaded = -1;
    bool shading = false;
    Matrix3D reProjectMatrix;

    // todo here transform matrix & shadow-zbuffer
};


#endif //BIG3DFLUFFY_QRLIGHTMANAGER_H
