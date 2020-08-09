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
    ~QRenderer() {
        delete[] cutters;}
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
    void threadManagePolygons(size_t size, int offset, int step, int thread_num=0);

    int thread_cnt = RENDER_THREAD_CNT;
    PolyRectCutter *cutters;

    QRLightManager *colorManager;
    QRasterizeZBuffer zbuf;

    QRImage *image;
    QRPolyScene3D *scene;
    QRCamera3D *camera;
    QRPolyModel3D *model;
    QRPolygon3D *poly;

    QRTransformer3D *cameraTransformer, *projector, *modelTransformer, *imageTransformer;
    Transformer3D modelCameraTransformer;

    Vector3D screenData, transZero;

    size_t points_cnt, polys_cnt;
    sptr<QRPolygon3D>* polys;
    sptr<QRPoint3D>* model_pts;
    QRVector<Vector3D> old_vectors, normals;
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
