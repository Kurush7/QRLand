//
// Created by kurush on 29.05.2020.
//

#ifndef KG_LAB3FACADE_H
#define KG_LAB3FACADE_H

#include "objects/objects.h"

#include "managers/QRManagers.h"
#include "QRPainter.h"


class Lab3Facade {
public:
    Lab3Facade(sptr<BasePainterCreator> cr);
    void draw();
    void addModel(std::string filename);
    void select(double x, double y);

    void move(double dx, double dy, double dz);
    void scale(double kx, double ky, double kz);
    void rotate(double dx, double dy, double dz);

    void deleteSelection();
    void undo();

    void setColor(QRColor pointcolor, QRColor edgeColor);

    bool isEmptySelection() {return scene->isEmptySelection();}

private:
    sptr<BasePainterCreator> painterCreator;
    sptr<BaseCommandManager> manager;
    sptr<QRObject3DFactory> factory;
    sptr<QRScene3D> scene;

};


#endif //KG_LAB3FACADE_H
