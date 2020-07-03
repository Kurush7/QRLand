//
// Created by kurush on 30.06.2020.
//

#ifndef BIG3DFLUFFY_QRENDERER_H
#define BIG3DFLUFFY_QRENDERER_H

#include "2Domain.h"
#include "3Domain/objects/objects.h"
#include "QRasterizeZBuffer.h"

class QRenderer {
public:
    QRenderer(const sptr<QRImage> &img, const sptr<QRPolyScene3D> &scene);
    void render();

    void setImage(const sptr<QRImage> &img) {image = img.get();}  // todo
    void setScene(const sptr<QRPolyScene3D> &s) {scene = s.get();}    // todo

private:
    QRImage *image;
    QRPolyScene3D *scene;
    QRColorManager *colorManager;
    QRasterizeZBuffer zbuf;

    // render data
    QRCamera3D *camera;
    QRPolygon3D *poly;
    QRTransformer3D *cameraTransformer, *projector, *modelTransformer, *imageTransformer;
    Vector3D screenData;
    QRPolyModel3D *model;
    QRVector<QRPolygon3D*> front;
    QRVector<QRPoint3D*> points;
    size_t points_cnt, front_size;
    QRVector<Vector3D> old_points;

};

class RenderCreator {
public:
    RenderCreator(const sptr<QRImage> &img, const sptr<QRPolyScene3D> &scene) {
        render = sptr<QRenderer>(new QRenderer(img, scene));
    }
    const sptr<QRenderer>& create() {return render;}
private:
    sptr<QRenderer> render;
};

#endif //BIG3DFLUFFY_QRENDERER_H
