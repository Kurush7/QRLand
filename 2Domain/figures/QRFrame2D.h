//
// Created by kurush on 19.08.2020.
//

#ifndef BIG3DFLUFFY_QRFRAME2D_H
#define BIG3DFLUFFY_QRFRAME2D_H

#include "QRLine2D.h"

void drawLine(const sptr<QRImage> image, const Vector3D &a, const Vector3D &b);

class QRFrame2D: public QRFigure2D {
public:

    QRFrame2D() = default;
    QRFrame2D(const QRVector<Vector3D> &points, QRColor _color=QRColor("black")): points(points) {
        color = _color;
        for (int i = 0; i < points.getSize() - 1; ++i)
            edges.push_back(QRLine2D(points[i], points[i+1], color));
        edges.push_back(QRLine2D(points[points.getSize()-1],
                points[0], color));
    }

    QRVector<QRLine2D> edges;
    QRVector<Vector3D> points;

    virtual void draw(const sptr<QRImage> &img) {
        for (auto &x: edges)
            x.draw(img);
    }

    virtual void setColor(QRColor c) {
        color = c;
        for (auto &x: edges)
            x.setColor(c);
    }

    virtual void setTransformer(const sptr<QRTransformer2D>& t) {
        transformer = t;
        for (auto &x: edges)
            x.setTransformer(t);
    }
};


#endif //BIG3DFLUFFY_QRFRAME2D_H
