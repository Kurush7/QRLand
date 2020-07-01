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
    explicit QRasterizeZBuffer(const sptr<QRImage> &img, const LightIterator &lights)
    : img(img), lights(lights) {
        w = img->getWidth();
        h = img->getHeight();
        zbuf = (double**) malloc (sizeof(double*) * h);
        clearBuf();
    }
    ~QRasterizeZBuffer() {
        for (int i = 0; i < h; ++i)
            free(zbuf[i]);
        free(zbuf);
    }

    // data may be spoiled! (reversed and rounded, both guaranteed!)
    void draw(RenderPolygon &poly, const sptr<QRTexture> &texture, const Vector3D &normal);

private:
    int w, h;
    double **zbuf;
    sptr<QRImage> img;
    sptr<QRTexture> texture;
    LightIterator lights;
    Vector3D normal;

    void fillRow(int xl, int xr, int y, int zl, int zr);
    void clearBuf();
};


#endif //BIG3DFLUFFY_QRASTERIZEZBUFFER_H
