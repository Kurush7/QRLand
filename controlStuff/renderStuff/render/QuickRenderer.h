//
// Created by kurush on 09.08.2020.
//

#ifndef BIG3DFLUFFY_QUICK_RENDERER_H
#define BIG3DFLUFFY_QUICK_RENDERER_H

#include "../QRenderer.h"
#include "QuickRenderData.h"
#include "../cutters/Quick3DCutter.h"

class QuickRenderer: public QRenderer {
public:
    QuickRenderer(const sptr<QRImage> &img, const sptr<QRPolyScene3D> &scene);
    ~QuickRenderer() {
        delete[] cutters;}
    virtual void render();

private:
    void initRender();
    // returns true if smth of the model is still visible. model field holds it
    bool modelCameraCut();
    void threadManagePolygons(size_t size, int offset, int step, int thread_num=0);

    int thread_cnt = RENDER_THREAD_CNT;
    PolyRectCutter *cutters;

    QRLightManager *colorManager;
    QRasterizeZBuffer zbuf;

    QuickRenderData quickData;

    QRCamera3D *camera;
    QRPolyModel3D *model;

    QRTransformer3D *cameraTransformer, *modelTransformer, *imageTransformer;
    Transformer3D modelCameraTransformer, projector;

    Vector3D screenData, transZero;

    sptr<QRPolygon3D>* polygons;
    sptr<QRPoint3D>* points;
    QRVector<sptr<QRPolygon3D>> local_polys;
    size_t polygon_cnt, point_cnt;

    size_t polysCameraCut=0, polysFrameCut=0;
};



#endif //BIG3DFLUFFY_QUICK_RENDERER_H
