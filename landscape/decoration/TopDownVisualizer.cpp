//
// Created by kurush on 18.08.2020.
//

#include "TopDownVisualizer.h"

void TopDownVisualizer::drawHeightMap() {
    size_t w = img->getWidth(), h = img->getHeight();
    auto hmap = builder->getHeightMap();
    size_t w_old = hmap.width(), h_old = hmap.height();

    float step = builder->getWorldStep();
    float max_h = -QRINF, min_h = QRINF;
    for (auto x: hmap)
        max_h = max(max_h, x),
        min_h = min(min_h, x);

    float kw = w / (w_old+0.), kh = h / (h_old + 0.);
    float kc = 255.f / (max_h - min_h + 0.);
    transformer = sptr<QRTransformer2D>(new ScaleTransformer2D(kw/step, kh/step));

    img->refillBg();


    /*size_t i, j, c;
    for (size_t i_old = 0; i_old < h_old; ++i_old)
        for (size_t j_old = 0; j_old < w_old; ++j_old) {
            i = i_old * kh, j = j_old * kw;
            c = (hmap[i_old][j_old] - min_h) * kc;
            img->mixPixel(i, j, QRColor(c, c, c));
        }*/

    size_t i_old, j_old, c;
    for (size_t i = 0; i < h; ++i)
        for (size_t j = 0; j < w; ++j) {
            i_old = i / kh, j_old = j / kw;
            c = (hmap[i_old][j_old] - min_h) * kc;
            img->mixPixel(i, j, QRColor(c, c, c));
        }


    for (auto &f: figures) {
        f->setTransformer(transformer);
        f->draw(img);
    }

    img->repaint();
}