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
    QuickRenderer(const sptr<QRImage> &img, QRPolyScene3D *scene);
    ~QuickRenderer() {
        delete cameraTransformer;
    }
    virtual void render();

    virtual void setUseShades(bool x) {
        useShades = x;}

protected:
    friend class QuickShadowRenderer;

    virtual void initRender();
    virtual bool modelCameraCut();    // returns true if smth of the model is still visible. model field holds it
    virtual void getPolygons();
    virtual void prepareData();
    virtual void cameraCut();
    virtual void project();
    virtual void rasterize();
    virtual void repaint();
    virtual void printRenderTimes();

    void threadDrawPolygons(int thread_num=0);
    void threadCutPolygons(size_t size, int offset, int thread_num=0);
    int thread_cnt = RENDER_THREAD_CNT;

    QRVector<sptr<Quick3DCutter>> cutters;

    QRasterizeZBuffer zbuf;

    QRCamera3D *camera;
    Vector3D cameraViewVector;
    QRPolyModel3D *model = nullptr;
    QRTransformer3D *cameraTransformer = nullptr, *modelTransformer, *imageTransformer;
    Transformer3D modelCameraTransformer, projector;

    QuickRenderMetaData data;

    // model data
    sptr<QRPolygon3D>* polygons;
    sptr<QRPoint3D>* points;
    QRVector<sptr<QRPolygon3D>> local_polys;
    size_t polygon_cnt, point_cnt;
    Vector3D screenData, transZero;
    // metrics & logging
    std::string polygonData;

    bool useShades=false;
};



#endif //BIG3DFLUFFY_QUICK_RENDERER_H
