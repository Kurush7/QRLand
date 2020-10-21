//
// Created by kurush on 19.08.2020.
//

#include "DrawLine.h"

void drawLine(const sptr<QRImage> image, const Vector3D &a, const Vector3D &b,
              const QRColor &color) {
    float dx = b[0] - a[0], dy = b[1] - a[1];
    float Dx = fabs(dx), Dy = fabs(dy);
    float L =  Dx > Dy ? Dx : Dy;
    dx /= L;
    dy /= L;
    float x = a[0], y = a[1];

    int ll = ceil(L);
    int xi, yi;
    for (int i = 0; i <= ll; ++i) {
        xi = round(x), yi = round(y);
        if (xi >= 0 && xi < image->getWidth() && yi >=0 && yi < image->getHeight())
            image->setPixel(xi, yi, color);
        x += dx, y += dy;
    }
}