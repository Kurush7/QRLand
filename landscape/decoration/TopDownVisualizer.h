//
// Created by kurush on 18.08.2020.
//

#ifndef BIG3DFLUFFY_TOPDOWNVISUALIZER_H
#define BIG3DFLUFFY_TOPDOWNVISUALIZER_H

#include "../managers/LandscapeBuilder.h"
#include "2Domain/2Domain.h"

class TopDownVisualizer {
public:
    TopDownVisualizer(const sptr<LandscapeBuilder> &builder,
            const sptr<QRImage> &img): builder(builder), img(img) {
        img->setBg(QRColor(0,0,0));
    }

    void addFigure(const sptr<QRFigure2D> &figure) {figures.push_back(figure);}

    void drawMiniMap();
    void setDrawHMap(bool x) {draw_hmap = x;}
    void setDrawWater(bool x) {drawWater = x;}
    void setDrawPlates(bool x) {drawPlates = x;}

private:
    const sptr<LandscapeBuilder> &builder;
    const sptr<QRImage> &img;

    bool draw_hmap = true;
    bool drawWater = false;
    bool drawPlates = true;

    QRVector<sptr<QRFigure2D>> figures;
    sptr<QRTransformer2D> transformer = nullptr;
};


#endif //BIG3DFLUFFY_TOPDOWNVISUALIZER_H
