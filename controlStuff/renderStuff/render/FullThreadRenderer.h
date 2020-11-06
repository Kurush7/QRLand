//
// Created by kurush on 09.08.2020.
//

#ifndef BIG3DFLUFFY_FULLTHREADRENDERER_H
#define BIG3DFLUFFY_FULLTHREADRENDERER_H

#include "../QRenderer.h"

class FullThreadRenderer: public QRenderer {
public:
    FullThreadRenderer(const sptr<QRImage> &img, const sptr<QRPolyScene3D> &scene);
    ~FullThreadRenderer() {
        delete[] cutters;}
    virtual void render();

private:
    void initRender();
    // returns true if smth of the model is still visible. model field holds it
    bool modelCameraCut();
    void threadManagePolygons(size_t size, int offset, int step, int thread_num=0);

    int thread_cnt = RENDER_THREAD_CNT;
    PolyRectCutter *cutters;

    QRasterizeZBuffer zbuf;

    QRCamera3D *camera;
    QRPolyModel3D *model;

    QRTransformer3D *cameraTransformer, *projector, *modelTransformer, *imageTransformer;
    Transformer3D modelCameraTransformer;

    Vector3D screenData, transZero;

    sptr<QRPolygon3D>* polygons;
    QRVector<sptr<QRPolygon3D>> local_polys;

    size_t polysCameraCut=0, polysFrameCut=0;
};



#endif //BIG3DFLUFFY_FULLTHREADRENDERER_H
