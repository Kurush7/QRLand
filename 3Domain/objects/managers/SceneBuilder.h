//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_SCENEBUILDER_H
#define BIG3DFLUFFY_SCENEBUILDER_H

#include "../composites/interfaces/QRScene3D.h"
#include "../components/interfaces/QRIndustry.h"
#include "../composites/legacy/SimpleScene3D.h"

class BaseScene3DBuilder {
public:
    virtual bool makeCamera() = 0;
    virtual bool placeElements() {stage++; return true;}

    virtual sptr<QRScene3D> getScene() = 0;
    virtual bool isReady() {return stage == built_stage;}

protected:
    int stage = 0, built_stage = 2;
    bool isBuilt = false;
    sptr<QRScene3D> scene;
};

class Scene3DBuilder: public BaseScene3DBuilder {
public:
    Scene3DBuilder (sptr<QRObject3DFactory>f, float width = 10, float height = 10);
    virtual bool makeCamera();
    virtual sptr<QRScene3D> getScene();
protected:
    sptr<QRObject3DFactory> factory;
    sptr<QRCamera3D> camera;
private:
    float w, h;
};


class SceneBuildDirector {
public:
    sptr<QRScene3D> build(sptr<BaseScene3DBuilder> loader)
    {
        if (loader->makeCamera() && loader->placeElements()) return loader->getScene();

        return nullptr;
    }
};


#endif //BIG3DFLUFFY_SCENEBUILDER_H
