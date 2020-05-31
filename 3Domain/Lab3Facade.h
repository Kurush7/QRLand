//
// Created by kurush on 29.05.2020.
//

#ifndef KG_LAB3FACADE_H
#define KG_LAB3FACADE_H

#include "objects/QRObjects.h"
#include "managers/QRManagers.h"
#include "Painter.h"

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
