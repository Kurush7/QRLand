//
// Created by kurush on 19.08.2020.
//

#ifndef BIG3DFLUFFY_QRFRAME2D_H
#define BIG3DFLUFFY_QRFRAME2D_H

#include "QRLine2D.h"
#include "algos/LineIntersection.h"

void drawLine(const sptr<QRImage> image, const Vector3D &a, const Vector3D &b);

class QRFrame2D: public QRFigure2D {
public:

    QRFrame2D() = default;
    QRFrame2D(const QRVector<Vector3D> &points, QRColor _color=QRColor("black")): points(points) {
        color = _color;
        updateEdges();

        center = ZeroVector;
        for (auto &p: points)
            center += p;
        center /= points.getSize();
    }

    QRVector<QRLine2D> edges;
    QRVector<Vector3D> points;
    Vector3D center;

    void updateEdges() {
        edges.clear();
        for (int i = 0; i < points.getSize() - 1; ++i)
            edges.push_back(QRLine2D(points[i], points[i+1], color));
        edges.push_back(QRLine2D(points[points.getSize()-1],
                                 points[0], color));
    }

    virtual void draw(const sptr<QRImage> &img) {
        for (auto &x: edges)
            x.draw(img);
    }

    Vector3D getCenter() {
        return center;
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

    Vector3D rayCenterIntersect(Vector3D dir) {
        float minDir = 1e9;
        Vector3D pt;
        for (int i = 0; i < edges.getSize(); ++i) {
            QRLine2D e = edges[i];
            char status;
            Vector3D inter = intersectLines(QRLine2D(center, center+dir).getEq(), e.getEq(), &status);
            if (status != 1) continue;
            if (cos2(dir, inter - center) < 0) continue;
            if (vectorLen(center - inter) < minDir) {
                minDir = vectorLen(center - inter);
                pt = inter;
            }
        }
        return pt;
    }

};


#endif //BIG3DFLUFFY_QRFRAME2D_H
