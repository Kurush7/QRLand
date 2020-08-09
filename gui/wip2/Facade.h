//
// Created by kurush on 29.05.2020.
//

#ifndef KG_LAB3FACADE_H
#define KG_LAB3FACADE_H

#include "objects/objects.h"
#include "managers/QRManagers.h"
#include "landscape/QRLandscape.h"
#include "renderStuff.h"

class Facade {
public:
    Facade(sptr<QRImage> img);
    void draw();

    void moveCamera(float dx, float dy, float dz);
    void scaleCamera(float kx, float ky, float kz);
    void rotateCamera(float dx, float dy, float dz);

    void changeCamera();

    void undo();

private:
    sptr<QRImage> image;
    sptr<BaseCommandManager> manager;
    sptr<QRPolyScene3D> scene;
    sptr<QRenderer> renderer;
};


#endif //KG_LAB3FACADE_H
