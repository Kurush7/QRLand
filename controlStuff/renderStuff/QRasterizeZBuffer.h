//
// Created by kurush on 30.06.2020.
//

#ifndef BIG3DFLUFFY_QRASTERIZEZBUFFER_H
#define BIG3DFLUFFY_QRASTERIZEZBUFFER_H


#include <containers/QRVector/QRVector.h>
#include "math/QRMath.h"
#include "QRPolyRectCutter.h"
#include "2Domain.h"

class QRasterizeZBuffer {
public:
    explicit QRasterizeZBuffer(const sptr<QRImage> &img): img(img) {
        w = img->getWidth();
        h = img->getHeight();
        auto c = QRColor("black");
        zbuf = (double**) malloc (sizeof(double*) * h);
        for (int i = 0; i < h; ++i) {
            zbuf[i] = (double *) malloc(sizeof(double) * w);
            for (int j = 0; j < w; ++j) {
                zbuf[i][j] = -QRINF;
                img->setPixel(i, j, c);
            }
        }
    }
    ~QRasterizeZBuffer() {
        for (int i = 0; i < h; ++i)
            free(zbuf[i]);
        free(zbuf);
    }

    // data may be spoiled! (reversed and rounded, both guaranteed!)
    void draw(RenderPolygon &poly, const sptr<QRTexture> &texture);

private:
    int w, h;
    double **zbuf;
    sptr<QRImage> img;
    sptr<QRTexture> texture;

    void fillRow(int xl, int xr, int y);
};


#endif //BIG3DFLUFFY_QRASTERIZEZBUFFER_H
