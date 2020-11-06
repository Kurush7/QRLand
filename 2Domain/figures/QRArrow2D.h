//
// Created by kurush on 21.10.2020.
//

#ifndef BIG3DFLUFFY_QRARROW2D_H
#define BIG3DFLUFFY_QRARROW2D_H

#include "QRFigure2D.h"
#include "../algos/DrawLine.h"

class QRArrow2D: public QRFigure2D {
public:

    QRArrow2D() = default;
    QRArrow2D(Vector3D a, Vector3D b, QRColor _color = QRColor("black")): a(a), b(b) {
        color = _color;
        updateEq();
    }

    Vector3D a, b;
    double ka, kb, kc;
    void updateEq() {
        ka = a[1] - b[1];
        kb = b[0] - a[0];
        kc = -ka*a[0] - kb*a[1];
    }
    Vector3D getEq() {return Vector3D(ka, kb, kc);}
    Vector3D getVector() {return b - a;}
    Vector3D getNormal() {return len2Norm(Vector3D(ka, kb, 0,0));}

    virtual void draw(const sptr<QRImage> &img) {
        Vector3D b1 = a + (b-a)*0.8;
        Vector3D b2 = b1 + getNormal() * vectorLen(b-b1)/4;
        b1 = b1 - getNormal() * vectorLen(b-b1)/4;

        Vector3D p = transformer->transform(a), q = transformer->transform(b);
        drawLine(img, p, q, color);

        b1 = transformer->transform(b1);
        b2 = transformer->transform(b2);
        drawLine(img, b1, q, color);
        drawLine(img, b2, q, color);
        drawLine(img, b1, b2, color);
    }
};

#endif //BIG3DFLUFFY_QRARROW2D_H
