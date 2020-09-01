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
    ~QuickRenderer() {}
    virtual void render();

private:
    void initRender();
    bool modelCameraCut();    // returns true if smth of the model is still visible. model field holds it
    void getPolygons();
    void prepareData();
    void cameraCut();
    void project();
    void rasterize();
    void repaint();
    void printRenderTimes();

    void threadDrawPolygons(size_t size, int offset, int step, int thread_num=0);
    void threadCutPolygons(size_t size, int offset, int step, int thread_num=0);
    int thread_cnt = RENDER_THREAD_CNT;

    QRVector<sptr<Quick3DCutter>> cutters;

    QRLightManager *colorManager;
    QRasterizeZBuffer zbuf;

    QRCamera3D *camera;
    QRPolyModel3D *model;
    QRTransformer3D *cameraTransformer, *modelTransformer, *imageTransformer;
    Transformer3D modelCameraTransformer, projector;

    QuickRenderData data;

    // model data
    sptr<QRPolygon3D>* polygons;
    sptr<QRPoint3D>* points;
    QRVector<sptr<QRPolygon3D>> local_polys;
    size_t polygon_cnt, point_cnt;
    Vector3D screenData, transZero;

    // metrics & logging
    std::string polygonData;
};



#endif //BIG3DFLUFFY_QUICK_RENDERER_H
