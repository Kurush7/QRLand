//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_SCENEDRAWMETHOD_H
#define BIG3DFLUFFY_SCENEDRAWMETHOD_H

#include "QRPainter.h"
#include "objects/objects.h"
#include "../../containers/QRContainers.h"
#include "DrawVisitors.h"

class BaseSceneDrawMethod {
public:
    virtual void draw(sptr<QRScene3D> s, sptr<QRImage> p) = 0;
protected:
    virtual void collectMetaData() = 0;
    virtual void collectPlainData() = 0;
    virtual void parsePlainData() = 0;
    virtual void paint() = 0;

    sptr<QRScene3D> scene;
};


class SceneDrawMethod {
public:
    virtual void draw(sptr<QRScene3D> s, sptr<QRImage> p);
protected:
    virtual void collectMetaData();
    virtual void collectPlainData();
    virtual void parsePlainData();
    virtual void paint();
private:
    QRVector<sptr<DrawableData>> drawableData;
    sptr<QRTransformer3D> transformer;
    sptr<QRScene3D> scene;
    sptr<QRImage> painter;
};

#endif //BIG3DFLUFFY_SCENEDRAWMETHOD_H
