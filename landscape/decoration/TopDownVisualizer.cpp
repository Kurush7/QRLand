//
// Created by kurush on 18.08.2020.
//

#include "TopDownVisualizer.h"

void TopDownVisualizer::drawMiniMap() {
    size_t w = img->getWidth(), h = img->getHeight();
    auto hmap = builder->getHeightMap();
    auto waterLevel = builder->waterManager->getWaterMatrix();
    size_t w_old = hmap.width(), h_old = hmap.height();
    float step = builder->getWorldStep();
    size_t i_old, j_old, c;

    img->refillBg();

    if (draw_hmap) {
        float max_h = -QRINF, min_h = QRINF;
        for (auto x: hmap)
            max_h = max(max_h, x),
                    min_h = min(min_h, x);

        float kw = w / (w_old + 0.), kh = h / (h_old + 0.);
        float kc = 255.f / (max_h - min_h + 0.);
        transformer = sptr<QRTransformer2D>(new ScaleTransformer2D(kw / step, kh / step));

        for (size_t i = 0; i < h; ++i)
            for (size_t j = 0; j < w; ++j) {
                i_old = i / kh, j_old = j / kw;
                c = (hmap[i_old][j_old] - min_h) * kc;
                img->mixPixel(j, h-1-i, QRColor(c, c, c));
            }
    }

    if (drawWater) {
        float max_h = -QRINF, min_h = QRINF;
        for (auto x: waterLevel)
            max_h = max(max_h, x),
                    min_h = min(min_h, x);
        float kw = w / (w_old+0.), kh = h / (h_old + 0.);
        float kc = 255.f / (max_h - min_h + 0.);
        for (size_t i = 0; i < h; ++i)
            for (size_t j = 0; j < w; ++j) {
                i_old = i / kh, j_old = j / kw;
                c = (waterLevel[i_old][j_old] - min_h) * kc;
                img->mixPixel(j, h-1-i, QRColor(0, 0, c));
            }
    }

    if (drawPlates) {
        for (auto &f: figures) {
            f->setTransformer(transformer);
            f->draw(img);
        }
    }

    img->repaint();
}