//
// Created by kurush on 30.06.2020.
//

#ifndef BIG3DFLUFFY_LIGHTS_H
#define BIG3DFLUFFY_LIGHTS_H

#include <iostream>

#include "math/QRMath.h"
#include "QRLightInterface.h"

const Vector3D defaultAmb = {0.2,0.2,0.2};
const Vector3D defaultDiff = {0.8,0.8,0.8};
const Vector3D defaultSpec = {0.0,0.0,0.0};

class BaseLight: public QRLight {
public:
    BaseLight(const Vector3D &pos, const Vector3D dir = XVector+ZVector,
            const Vector3D &amb = defaultAmb, const Vector3D &diff = defaultDiff,
            const Vector3D &spec = defaultSpec)
              : pos(pos), ambient(amb), diffuse(diff), specular(spec), lightDir(dir) {}

    virtual Vector3D getPosition() {return pos;}
    virtual Vector3D getIntensity(const Vector3D &pos, const Vector3D &normal) {
        Vector3D i = ZeroVector + ambient;
        float x = cos3(normal, lightDir);
        if (x >= 0)
            i += diffuse * x;    // -1: angle between returning light, not falling...
        return i;
        // todo: divide by (distance + constant), add specular, manage with fabs
    }


    virtual void transform(const QRTransformer3D *trans) {
        pos0 = pos;
        pos = trans->transform(pos);
        cout << "light pos: " << pos << '\n';
    }
    virtual void resetPosition() {
        pos = pos0;
    }

private:
    Vector3D pos, pos0, lightDir;
    Vector3D ambient, diffuse, specular;
};


#endif //BIG3DFLUFFY_LIGHTS_H
