//
// Created by kurush on 29.05.2020.
//

#ifndef KG_LAB3FACADE_H
#define KG_LAB3FACADE_H

#include "objects/objects.h"
#include "managers/QRManagers.h"
#include "renderStuff.h"

class Facade {
public:
    Facade(sptr<QRImage> img);
    void draw();
    void addCube(double a, double x, double y, double z, QRColor c);

    void moveCamera(double dx, double dy, double dz);
    void scaleCamera(double kx, double ky, double kz);
    void rotateCamera(double dx, double dy, double dz);

    void undo();

private:
    sptr<QRImage> image;
    sptr<BaseCommandManager> manager;
    sptr<QRPolyScene3D> scene;
    sptr<QRenderer> renderer;
};


#endif //KG_LAB3FACADE_H
