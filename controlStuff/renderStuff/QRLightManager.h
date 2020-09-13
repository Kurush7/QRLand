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

    void ambientLight(QRColor &c) {
        Vector3D intens = ZeroVector;
        // todo optimize: precompute all ambiences

        for (int i = 0; i < size; ++i)
            intens += lights[i]->getAmbient();
        c.r = c.r * intens[0];
        c.g = c.g * intens[1];
        c.b = c.b * intens[2];
    }

    void lightenColor(const Vector3D &pos, const Vector3D &normal, const QRColor &c, QRColor &res) {
        Vector3D intens = ZeroVector;
        // todo optimize: precompute all ambiences

        /*if (shading) {
            Vector3D v = reProjectMatrix * pos;
            int x = round(v[0]), y = round(v[1]);
            //cout << x << ' ' << y << ' ' << v[2] << " => " << shadeZBuf[y*shadeW + x] << '\n';
            if (shadeZBuf[y*shadeW + x] < v[2])
                intens = lights[lightShaded]->getAmbient();
            else
                for (int i = 0; i < size; ++i)
                    intens += lights[i]->getIntensity(pos, normal);
        }
        else*/
        for (int i = 0; i < size; ++i)
            intens += lights[i]->getIntensity(pos, normal);

        res.r = intens[0] * c.r;
        res.g = intens[1] * c.g;
        res.b = intens[2] * c.b;
        // todo alpha here....)))
    }

    virtual bool isShaded(const float *p) {
        return true;
        Vector3D v(p[0], p[1], p[2]);
        v = reProjectMatrix * v;
        int x = round(v[0]), y = round(v[1]);
        return shadeZBuf[y*shadeW + x] < v[2] - QREPS;
    }

    virtual bool isShaded(const Vector3D &p) {
        Vector3D v = p;
        v = reProjectMatrix * v;
        int x = round(v[0]), y = round(v[1]);
        return shadeZBuf[y*shadeW + x] < v[2] - QREPS;
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

    void setTransformTo(const Matrix3D &globalToLightProj) {
        to = globalToLightProj;
        reProjectMatrix = to * from;
        cout << "to " << reProjectMatrix << '\n';
    }
    void setTransformFrom(const Matrix3D &camProjToGlobal) {
        from = camProjToGlobal;
        reProjectMatrix = to * from;
        cout << "from " << reProjectMatrix << '\n';
    }

    void useShades(bool x=true) {shading = x;}

    Matrix3D reProjectMatrix;   // todo for debug
private:
    QRLight** lights;
    int size = 0;

    float *shadeZBuf = nullptr;
    int shadeW=0, shadeH=0, maxShadeSize=0;
    int lightShaded = -1;
    bool shading = false;
    Matrix3D from = makeID(), to = makeID();

    // todo here transform matrix & shadow-zbuffer
};


#endif //BIG3DFLUFFY_QRLIGHTMANAGER_H
