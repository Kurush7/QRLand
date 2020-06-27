//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_FRAMELOADER_H
#define BIG3DFLUFFY_FRAMELOADER_H

#include "../composites/interfaces/QRFrame3D.h"
#include "../composites/legacy/SimpleFrame3D.h"
#include "../components/interfaces/QRIndustry.h"
#include "LoadSource.h"


class BaseFrame3DLoader {
public:
    virtual bool loadPoints() = 0;
    virtual bool loadEdges() = 0;

    virtual sptr<QRFrame3D> getFrame() = 0;
    virtual bool isReady() {return stage == built_stage;}

protected:

    int stage = 0, built_stage = 2;
    bool isBuilt = false;
    sptr<QRFrame3D> frame;

};

class Frame3DLoader: public BaseFrame3DLoader {
public:
    Frame3DLoader (sptr<LoadSource> src,  sptr<QRObject3DFactory>f);
    virtual bool loadPoints();
    virtual bool loadEdges();
    virtual sptr<QRFrame3D> getFrame();
protected:
    sptr<LoadSource> source;
    sptr<QRObject3DFactory> factory;
    QRVector<sptr<QRPoint3D>> points;
    QRVector<sptr<QRObject>> objects;
};


class FrameLoadDirector {
public:
    sptr<QRFrame3D> load(sptr<BaseFrame3DLoader> loader)
    {
        if (loader->loadPoints() && loader->loadEdges()) return loader->getFrame();

        return nullptr;
    }
};

#endif //BIG3DFLUFFY_FRAMELOADER_H
