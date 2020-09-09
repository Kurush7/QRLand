//
// Created by kurush on 10.09.2020.
//

#ifndef BIG3DFLUFFY_QUICKSHADOWRENDERER_H
#define BIG3DFLUFFY_QUICKSHADOWRENDERER_H

#include "QuickRenderer.h"

// works for only on light source!
class QuickShadowRenderer: public QuickRenderer {
public:
    QuickShadowRenderer(const sptr<QRImage> &img, const sptr<QRPolyScene3D> &scene, int light_source_pos=0);
    virtual void render();
    void generateShades() {render();}

protected:
    virtual void initRender();
    virtual void prepareData();
    virtual void rasterize();

    void transformPoints();
    void threadTransformPoints(size_t size, int offset, int step, int thread_num=0);

    int light_source_pos;
};


#endif //BIG3DFLUFFY_QUICKSHADOWRENDERER_H
