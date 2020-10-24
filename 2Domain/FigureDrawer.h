//
// Created by kurush on 21.10.2020.
//

#ifndef BIG3DFLUFFY_FIGUREDRAWER_H
#define BIG3DFLUFFY_FIGUREDRAWER_H

#include "figures/QRFigure2D.h"
#include "transformers/ScaleTransformer.h"

class FigureDrawer {
public:
    FigureDrawer(const sptr<QRImage> &img): img(img) {
        img->setBg(QRColor(0,0,0));
    }

    void addFigure(const sptr<QRFigure2D> &figure) {figures.push_back(figure);}
    void clearFigures() {figures.clear();}

    void draw();

    void setScale(float w, float h) {kw = w, kh = h;}

    sptr<QRTransformer2D> transformer = nullptr;
private:
    const sptr<QRImage> &img;

    float kw = 1, kh = 1;
    QRVector<sptr<QRFigure2D>> figures;
};


#endif //BIG3DFLUFFY_FIGUREDRAWER_H
