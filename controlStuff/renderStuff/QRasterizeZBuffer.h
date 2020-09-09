//
// Created by kurush on 30.06.2020.
//

#ifndef BIG3DFLUFFY_QRASTERIZEZBUFFER_H
#define BIG3DFLUFFY_QRASTERIZEZBUFFER_H

#include <algorithm>
#include <mutex>

#include "containers/QRContainers.h"
#include "math/QRMath.h"#include "QRasterizeZBuffer.h"
#include "renderStuff/cutters/PolyRectCutter.h"
#include "2Domain.h"
#include "QRLightManager.h"

struct renderData {
    Vector3D *poly;
    const QRTexture *texture;
    QRColor c;
    int n;
};
class QRasterizeZBuffer {
public:
    explicit QRasterizeZBuffer(const sptr<QRImage> &_img, QRLightManager *man)
    : img(_img.get()), colorManager(man) {
        w = img->getWidth();
        h = img->getHeight();
        zbuf = (float*) malloc (sizeof(float*) * h*w);
        row_example = (float*) malloc (sizeof(float*) * w);
        for (int i = 0; i < w; ++i)
            row_example[i] = QRINF;
        clearZBufOnly();
    }
    ~QRasterizeZBuffer() {
        free(zbuf);
        free(row_example);
    }

    // data may be spoiled! (reversed and rounded, both guaranteed!)
    void draw(Vector3D *_poly, int size, const Vector3D &norm, const QRTexture *texture);
    void draw(float** points, int32_t* poly, int size, const Vector3D &norm, const QRTexture *texture);
    void clearBuf();
    void clearZBufOnly();
    void fillMissing();
private:
    int w, h;
    float *zbuf;
    float *row_example;
    QRImage *img;
    QRLightManager *colorManager;

    void drawTriangle(float p1x, float p1y, float zl,
            float p2x, float p2y, float zr,
            float p3x, float p3y, float zw, QRColor c);

    void fillRow(renderData &data);
    inline void jumpL(renderData &);
    inline void jumpR(renderData &);

};


#endif //BIG3DFLUFFY_QRASTERIZEZBUFFER_H
