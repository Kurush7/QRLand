//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_SCENEDRAWMETHOD_H
#define BIG3DFLUFFY_SCENEDRAWMETHOD_H

#include "../Painter.h"
#include "objects/objects.h"
#include "../containers/QRContainers.h"

class BaseSceneDrawMethod {
public:
    virtual void draw(sptr<BaseScene3D> s, sptr<Painter> p) = 0;
protected:
    virtual void collectMetaData() = 0;
    virtual void collectPlainData() = 0;
    virtual void parsePlainData() = 0;
    virtual void paint() = 0;

    sptr<BaseScene3D> scene;
};


class SceneDrawMethod {
public:
    virtual void draw(sptr<BaseScene3D> s, sptr<Painter> p);
protected:
    virtual void collectMetaData();
    virtual void collectPlainData();
    virtual void parsePlainData();
    virtual void paint();
private:
    QRVector<sptr<DrawableData>> drawableData;
    sptr<BaseTransformer3D> transformer;
    sptr<BaseScene3D> scene;
    sptr<Painter> painter;
};

#endif //BIG3DFLUFFY_SCENEDRAWMETHOD_H
