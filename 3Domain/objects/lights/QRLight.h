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
            : pos(pos), ambient(amb[0]), diffuse(diff[0]), specular(spec[0]), lightDir(len3Norm(dir)) {   // todo -1 ... hardcode for normal orientation
        bare_pos[0] = pos[0], bare_pos[1] = pos[1], bare_pos[2] = pos[2];
        bare_dir[0] = lightDir[0], bare_dir[1] = lightDir[1], bare_dir[2] = lightDir[2];
    }

    Vector3D getLightVector() {return lightDir;}
    Vector3D getPosition() {return pos;}

    // todo.... no color-masks. basic intencity common for all
    float getIntensity(const Vector3D &normal) {
        // todo control: normal & lightDir must be normed
        //float x = cos3(normal, lightDir);
        float x = bare_dir[0]*normal[0] + bare_dir[1]*normal[1] + bare_dir[2]*normal[2];
        if (x < 0) return ambient - diffuse * x;
        return ambient;

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

    float getAmbient() {return ambient;}

    void setLightDir(Vector3D v) {lightDir = len3Norm(v); for (int i = 0; i < 3; ++i) bare_dir[i] = lightDir[i];}
    void setLightPos(Vector3D v) {for (int i = 0; i < 3; ++i) bare_pos[i] = v[i]; pos = v, pos0 = v;}


private:
    float bare_pos[4], bare_dir[4];
    Vector3D pos, pos0, lightDir;
    float ambient, diffuse, specular;
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
