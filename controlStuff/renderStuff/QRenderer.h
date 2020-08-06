//
// Created by kurush on 30.06.2020.
//

#ifndef BIG3DFLUFFY_QRENDERER_H
#define BIG3DFLUFFY_QRENDERER_H

#include <thread>

#include "2Domain.h"
#include "3Domain/objects/objects.h"
#include "QRasterizeZBuffer.h"


#include "QRPolyRectCutter.h"
#include "QRasterizeZBuffer.h"

#include "tests/TimeTest.h"
#include <memory>

class QRenderer {
public:
    QRenderer(const sptr<QRImage> &img, const sptr<QRPolyScene3D> &scene);
    void render();

    void setImage(const sptr<QRImage> &img) {image = img.get();}  // todo
    void setScene(const sptr<QRPolyScene3D> &s) {scene = s.get();}    // todo

private:
    void initRender();
    // returns true if smth of the model is still visible. model field holds it
    bool modelCameraCut();
    void copyTransformPoints();
    void updateNormals();
    void projectPoints();
    void restorePoints();

    void frameCutDraw();
    void threadManagePolygons(sptr<QRPolygon3D>* polys, size_t size, int step, int thread_num=0);

    QRImage *image;
    QRPolyScene3D *scene;
    QRLightManager *colorManager;
    QRasterizeZBuffer zbuf;

    // render data
    QRCamera3D *camera;
    QRPolygon3D *poly;
    sptr<QRPolygon3D>* polys;
    sptr<QRPoint3D>* model_pts;
    QRTransformer3D *cameraTransformer, *projector, *modelTransformer, *imageTransformer;
    Vector3D screenData, transZero;
    QRPolyModel3D *model;
    PolyPosition vPlace;
    size_t points_cnt, polys_size;
    QRVector<Vector3D> old_vectors;
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
