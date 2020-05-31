//
// Created by kurush on 29.05.2020.
//

#ifndef KG_LAB3FACADE_H
#define KG_LAB3FACADE_H

#include "objects/QRObjects.h"
#include "managers/QRManagers.h"

class Painter {
public:
    virtual void drawPoint(double x, double y, QRPointStyle) = 0;
    virtual void drawEdge(double x1, double y1, double x2, double y2, QREdgeStyle) = 0;
};

class Lab3Facade {
public:
    Lab3Facade();
    void draw(std::shared_ptr<Painter>);
    void addModel(std::string filename);
private:
    std::shared_ptr<BaseCommandManager> manager;
    std::shared_ptr<AbstractObject3DFactory> factory;
    std::shared_ptr<BaseScene3D> scene;

};


#endif //KG_LAB3FACADE_H
