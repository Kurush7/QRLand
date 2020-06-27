//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_DRAWVISITORS_H
#define BIG3DFLUFFY_DRAWVISITORS_H

#include "objects/components/interfaces.h"
#include "objects/composites/interfaces.h"
#include "objects/QRObject.h"
#include "QRPainter.h"
#include "../../QRConstants.h"
#include "DrawData.h"

#include "objects/visitors/QRVisitorInterface.h"

std::ostream& operator<<(std::ostream &os, const DrawablePoint &p);
std::ostream& operator<<(std::ostream &os, const DrawableEdge &e);

class QRDrawVisitor {
public:
    virtual void visitDrawPoint(DrawablePoint &p) = 0;
    virtual void visitDrawEdge(DrawableEdge &e) = 0;
private:
    sptr<QRPainter> painter;
};

class DrawMethodVisitor: public QRDrawVisitor {
public:
    DrawMethodVisitor(sptr<QRPainter> painter): painter(painter) {}
    void visitDrawPoint(DrawablePoint &p) {
        //cout << p <<'\n';
        painter->drawPoint(p.x, p.y, p.style);
    }
    void visitDrawEdge(DrawableEdge &e) {
        //cout << e;
        painter->drawEdge(e.x1,e.y1, e.x2, e.y2, e.style);
    }
private:
    sptr<QRPainter> painter;
};

class HideInvisibleDrawMethodVisitor: public QRDrawVisitor {
public:
    HideInvisibleDrawMethodVisitor(double z_min): z_min(z_min) {}
    void visitDrawPoint(DrawablePoint &p) {
        //cout << "point-z:" << p.z << '\n';
        visible = p.z > z_min;
    }
    void visitDrawEdge(DrawableEdge &e) {
        //cout << e;
        visible = e.z1 > z_min && e.z2 > z_min;
    }

    bool visible;
private:
    double z_min;
};

class DrawVisitor: public QRVisitor {
public:
    DrawVisitor(sptr<BaseTransformer3D> t, QRVector<sptr<DrawableData>> &data,
                sptr<QRPainter> painter): transformer(t), data(data), painter(painter) {}
    virtual void visitPoint3D(sptr<QRPoint3D> point);
    virtual void visitEdge3D(sptr<QREdge3D> edge);
    virtual void visitCamera3D(sptr<QRCamera3D> camera);

private:
    sptr<BaseTransformer3D> transformer;
    QRVector<sptr<DrawableData>> &data;
    sptr<QRPainter> painter;
};

#endif //BIG3DFLUFFY_DRAWVISITORS_H
