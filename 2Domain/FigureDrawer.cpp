//
// Created by kurush on 21.10.2020.
//

#include "FigureDrawer.h"

void FigureDrawer::draw() {
    transformer = sptr<QRTransformer2D>(new ScaleTransformer2D(kw, kh));

    img->refillBg();

    for (auto &f: figures) {
        f->setTransformer(transformer);
        f->draw(img);
    }

    img->repaint();
}