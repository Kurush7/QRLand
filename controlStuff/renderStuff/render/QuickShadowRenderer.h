//
// Created by kurush on 10.09.2020.
//

#ifndef BIG3DFLUFFY_QUICKSHADOWRENDERER_H
#define BIG3DFLUFFY_QUICKSHADOWRENDERER_H

#include "QuickRenderer.h"

// works for only on light source!
class QuickShadowRenderer {
public:
    QuickShadowRenderer(sptr<QuickRenderer> &r, int light_source_pos=0);
    virtual void render();
    void generateShades(size_t newW, size_t newH) {gridW = newW, gridH = newH; render();}

protected:
    size_t gridW, gridH;

    void initRender();
    void prepareData();
    void rasterize();
    void addFakeBorders();

    void transformPoints();
    void threadTransformPoints(size_t size, int offset, int step, int thread_num=0);

    int light_source_pos;
    sptr<QuickRenderer> renderer;
    sptr<QRImage> image;
    QRVector<sptr<QRPolygon3D>> polys;
    QRVector<sptr<QRPoint3D>> points;

    sptr<QRCanvas> shadowCanvas;
    sptr<QRImage> shadowImage;
    QRImage *oldImage;
    sptr<QRasterizeZBuffer> shadowBuffer;

};


#endif //BIG3DFLUFFY_QUICKSHADOWRENDERER_H
