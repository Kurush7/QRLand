//
// Created by kurush on 09.08.2020.
//

#ifndef BIG3DFLUFFY_QRENDERER_H
#define BIG3DFLUFFY_QRENDERER_H

#include <thread>

#include "2Domain.h"
#include "3Domain/objects/objects.h"

#include "../QRasterizeZBuffer.h"
#include "renderStuff/cutters/PolyRectCutter.h"
#include "renderStuff/cutters/Poly3DCutter.h"
#include "renderStuff/renderConfig.h"

#include "tests/TimeTest.h"
#include <memory>

class QRenderer {
public:
    QRenderer(const sptr<QRImage> &img, const sptr<QRPolyScene3D> &scene)
    : image(img.get()), scene(scene.get()) {}

    virtual void render() = 0;

    virtual void setImage(const sptr<QRImage> &img) {image = img.get();}
    virtual void setScene(const sptr<QRPolyScene3D> &s) {scene = s.get();}

protected:
    QRImage *image;
    QRPolyScene3D *scene;
};

#endif //BIG3DFLUFFY_QRENDERER_H
