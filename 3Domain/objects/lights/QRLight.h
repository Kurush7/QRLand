//
// Created by kurush on 30.06.2020.
//

#ifndef BIG3DFLUFFY_QRLIGHT_H
#define BIG3DFLUFFY_QRLIGHT_H

#include "math/QRMath.h"

const Vector3D defaultAmb = {0.2,0.2,0.2};
const Vector3D defaultDiff = {0.8,0.8,0.8};
const Vector3D defaultSpec = {0.0,0.0,0.0};

class QRLight {
public:
    QRLight(const Vector3D &pos, const Vector3D dir = XVector+ZVector,
              const Vector3D &amb = defaultAmb, const Vector3D &diff = defaultDiff,
              const Vector3D &spec = defaultSpec)
            : pos(pos), ambient(amb), diffuse(diff), specular(spec), lightDir(dir) {}


    Vector3D getLightVector() {return lightDir;}

    Vector3D getPosition() {return pos;}
    Vector3D getIntensity(const Vector3D &pos, const Vector3D &normal) {
        Vector3D i = ZeroVector + ambient;
        float x = cos3(-1*normal, lightDir);
        if (x >= 0)
            i += diffuse * x;    // -1: angle between returning light, not falling...
        return i;
        // todo: divide by (distance + constant), add specular, manage with fabs
    }


    void transform(const QRTransformer3D *trans) {
        pos0 = pos;
        pos = trans->transform(pos);
        cout << "light pos: " << pos << '\n';
    }
    void resetPosition() {
        pos = pos0;
    }

    Vector3D& getAmbient() {return ambient;}

private:
    Vector3D pos, pos0, lightDir;
    Vector3D ambient, diffuse, specular;
};



class QRLightInterface {
public:
    virtual void transform(const QRTransformer3D *trans) = 0;
    virtual void resetPosition() = 0;
    virtual Vector3D getPosition() = 0;
    virtual Vector3D getLightVector() = 0;
    virtual Vector3D getIntensity(const Vector3D &pos, const Vector3D &normal) = 0;
};

#endif //BIG3DFLUFFY_QRLIGHT_H
