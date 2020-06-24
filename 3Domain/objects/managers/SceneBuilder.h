//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_SCENEBUILDER_H
#define BIG3DFLUFFY_SCENEBUILDER_H

#include "../composites/Scene3D.h"
#include "../components/Industry.h"

class BaseScene3DBuilder {
public:
    virtual bool makeCamera() = 0;
    virtual bool placeElements() {stage++; return true;}

    virtual std::shared_ptr<BaseScene3D> getScene() = 0;
    virtual bool isReady() {return stage == built_stage;}

protected:
    int stage = 0, built_stage = 2;
    bool isBuilt = false;
    std::shared_ptr<BaseScene3D> scene;
};

class Scene3DBuilder: public BaseScene3DBuilder {
public:
    Scene3DBuilder (std::shared_ptr<AbstractObject3DFactory>f, double width = 10, double height = 10);
    virtual bool makeCamera();
    virtual std::shared_ptr<BaseScene3D> getScene();
protected:
    std::shared_ptr<AbstractObject3DFactory> factory;
    std::shared_ptr<QRCamera3D> camera;
private:
    double w, h;
};


class SceneBuildDirector {
public:
    std::shared_ptr<BaseScene3D> build(shared_ptr<BaseScene3DBuilder> loader)
    {
        if (loader->makeCamera() && loader->placeElements()) return loader->getScene();

        return nullptr;
    }
};


#endif //BIG3DFLUFFY_SCENEBUILDER_H
