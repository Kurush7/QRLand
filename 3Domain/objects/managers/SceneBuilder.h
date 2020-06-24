//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_SCENEBUILDER_H
#define BIG3DFLUFFY_SCENEBUILDER_H

#include "../composites/Scene3D.h"
#include "../components/interfaces/QRIndustry.h"

class BaseScene3DBuilder {
public:
    virtual bool makeCamera() = 0;
    virtual bool placeElements() {stage++; return true;}

    virtual sptr<BaseScene3D> getScene() = 0;
    virtual bool isReady() {return stage == built_stage;}

protected:
    int stage = 0, built_stage = 2;
    bool isBuilt = false;
    sptr<BaseScene3D> scene;
};

class Scene3DBuilder: public BaseScene3DBuilder {
public:
    Scene3DBuilder (sptr<AbstractObject3DFactory>f, double width = 10, double height = 10);
    virtual bool makeCamera();
    virtual sptr<BaseScene3D> getScene();
protected:
    sptr<AbstractObject3DFactory> factory;
    sptr<QRCamera3D> camera;
private:
    double w, h;
};


class SceneBuildDirector {
public:
    sptr<BaseScene3D> build(sptr<BaseScene3DBuilder> loader)
    {
        if (loader->makeCamera() && loader->placeElements()) return loader->getScene();

        return nullptr;
    }
};


#endif //BIG3DFLUFFY_SCENEBUILDER_H
