//
// Created by kurush on 30.06.2020.
//

#ifndef BIG3DFLUFFY_QRASTERIZEZBUFFER_H
#define BIG3DFLUFFY_QRASTERIZEZBUFFER_H

#include <algorithm>

#include "containers/QRContainers.h"
#include "math/QRMath.h"
#include "QRPolyRectCutter.h"
#include "2Domain.h"
#include "QRColorManager.h"

class QRasterizeZBuffer {
public:
    explicit QRasterizeZBuffer(const sptr<QRImage> &_img, QRColorManager *man)
    : img(_img.get()), colorManager(man) {
        w = img->getWidth();
        h = img->getHeight();
        zbuf = (float**) malloc (sizeof(float*) * h);
        for (int i = 0; i < h; ++i)
            zbuf[i] = (float *) malloc(sizeof(float) * w);
        clearBuf();
    }
    ~QRasterizeZBuffer() {
        for (int i = 0; i < h; ++i)
            free(zbuf[i]);
        free(zbuf);
    }

    // data may be spoiled! (reversed and rounded, both guaranteed!)
    void draw(Vector3D *_poly, int size, const Vector3D &normal);
    void clearBuf();

private:
    int w, h;
    float **zbuf;
    QRImage *img;
    QRColorManager *colorManager;

    void fillRow();

    inline void jumpL();
    inline void jumpR();

    // draw data
    Vector3D *poly;
    QRColor c;
    int n;
    int dir;
    int left, right, ll, rr;     // ll - after left, rr - after right
    float bl, br, dzl, dzr;
    int xli, xri, y;
    float zl, zr, xl, xr;
};


#endif //BIG3DFLUFFY_QRASTERIZEZBUFFER_H
