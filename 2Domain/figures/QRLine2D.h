//
// Created by kurush on 19.08.2020.
//

#ifndef BIG3DFLUFFY_QRLINE2D_H
#define BIG3DFLUFFY_QRLINE2D_H

#include "QRFigure2D.h"
#include "../algos/DrawLine.h"

class QRLine2D: public QRFigure2D {
public:

    QRLine2D() = default;
    QRLine2D(Vector3D a, Vector3D b, QRColor _color = QRColor("black")): a(a), b(b) {
        color = _color;
        ka = a[1] - b[1];
        kb = b[0] - a[0];
        kc = -ka*a[0] - kb*a[1];
    }

    Vector3D a, b;
    double ka, kb, kc;
    Vector3D getVector() {return Vector3D(ka,kb,kc);}

    virtual void draw(const sptr<QRImage> &img) {
        drawLine(img, transformer->transform(a),
                transformer->transform(b), color);
    }
};


#endif //BIG3DFLUFFY_QRLINE2D_H
