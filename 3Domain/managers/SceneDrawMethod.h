//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_SCENEDRAWMETHOD_H
#define BIG3DFLUFFY_SCENEDRAWMETHOD_H

#include "../Painter.h"
#include "../objects/QRObjects.h"
#include "../containers/QRContainers.h"

class BaseSceneDrawMethod {
public:
    virtual void draw(std::shared_ptr<BaseScene3D> s, std::shared_ptr<Painter> p) = 0;
protected:
    virtual void collectMetaData() = 0;
    virtual void collectPlainData() = 0;
    virtual void parsePlainData() = 0;
    virtual void paint() = 0;

    std::shared_ptr<BaseScene3D> scene;
};


class SceneDrawMethod {
public:
    virtual void draw(std::shared_ptr<BaseScene3D> s, std::shared_ptr<Painter> p);
protected:
    virtual void collectMetaData();
    virtual void collectPlainData();
    virtual void parsePlainData();
    virtual void paint();
private:
    QRVector<std::shared_ptr<DrawableData>> drawableData;
    std::shared_ptr<BaseTransformer3D> transformer;
    std::shared_ptr<BaseScene3D> scene;
    std::shared_ptr<Painter> painter;
};

#endif //BIG3DFLUFFY_SCENEDRAWMETHOD_H
