//
// Created by kurush on 19.08.2020.
//

#ifndef BIG3DFLUFFY_QRFIGURE2D_H
#define BIG3DFLUFFY_QRFIGURE2D_H

#include "QRImage.h"
#include "../transformers/QRTransformer2D.h"

class QRFigure2D {
public:
    virtual void draw(const sptr<QRImage> &img) = 0;

    virtual void setTransformer(const sptr<QRTransformer2D>& t) {
        transformer = t;
    }

    virtual void setColor(QRColor c) {color = c;}

protected:
    sptr<QRTransformer2D> transformer = sptr<QRTransformer2D>(new QRTransformer2D);
    QRColor color;
};

#endif //BIG3DFLUFFY_QRFIGURE2D_H
