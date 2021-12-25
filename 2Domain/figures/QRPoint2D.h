//
// Created by kurush on 19.08.2020.
//

#ifndef BIG3DFLUFFY_QRPOINT2D_H
#define BIG3DFLUFFY_QRPOINT2D_H

#include "QRFigure2D.h"

class QRPoint2D: public QRFigure2D {
public:

    QRPoint2D() = default;
    QRPoint2D(Vector3D a, QRColor _color = QRColor("black")): a(a) {
        color = _color;
    }

    Vector3D a;

    virtual void draw(const sptr<QRImage> &img) {
        auto x = transformer->transform(a);
        img->setPixel(x[0], x[1], color);
    }
};


#endif //BIG3DFLUFFY_QRPOINT2D_H
