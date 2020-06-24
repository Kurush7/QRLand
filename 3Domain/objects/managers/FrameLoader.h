//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_FRAMELOADER_H
#define BIG3DFLUFFY_FRAMELOADER_H

#include "../composites/Frame3D.h"
#include "../components/interfaces/QRIndustry.h"
#include "LoadSource.h"


class BaseFrame3DLoader {
public:
    virtual bool loadPoints() = 0;
    virtual bool loadEdges() = 0;

    virtual std::shared_ptr<BaseFrame3D> getFrame() = 0;
    virtual bool isReady() {return stage == built_stage;}

protected:

    int stage = 0, built_stage = 2;
    bool isBuilt = false;
    std::shared_ptr<BaseFrame3D> frame;

};

class Frame3DLoader: public BaseFrame3DLoader {
public:
    Frame3DLoader (std::shared_ptr<LoadSource> src,  std::shared_ptr<AbstractObject3DFactory>f);
    virtual bool loadPoints();
    virtual bool loadEdges();
    virtual std::shared_ptr<BaseFrame3D> getFrame();
protected:
    std::shared_ptr<LoadSource> source;
    std::shared_ptr<AbstractObject3DFactory> factory;
    QRVector<std::shared_ptr<QRPoint3D>> points;
    QRVector<std::shared_ptr<QRObject>> objects;
};


class FrameLoadDirector {
public:
    std::shared_ptr<BaseFrame3D> load(shared_ptr<BaseFrame3DLoader> loader)
    {
        if (loader->loadPoints() && loader->loadEdges()) return loader->getFrame();

        return nullptr;
    }
};

#endif //BIG3DFLUFFY_FRAMELOADER_H
