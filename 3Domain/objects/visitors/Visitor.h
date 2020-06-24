//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_VISITOR_H
#define BIG3DFLUFFY_VISITOR_H


#include "../components/interfaces/QRPoint3D.h"
#include "../components/interfaces/QREdge3D.h"
#include "../components/interfaces/QRCamera3D.h"
#include "../composites/Frame3D.h"
#include "objects/QRObject.h"
#include "../../Painter.h"
#include "../../QRConstants.h"


class BaseDrawMethodVisitor;
struct DrawableData {
    virtual void acceptVisitor(sptr<BaseDrawMethodVisitor> &v) = 0;
};
struct DrawablePoint: public DrawableData {
    DrawablePoint(double x, double y, double z, QRPointStyle style): x(x), y(y), z(z), style(style) {}
    virtual void acceptVisitor(sptr<BaseDrawMethodVisitor> &v);
    double x, y, z;
    QRPointStyle style;
};
struct DrawableEdge: public DrawableData {
    DrawableEdge(double x1, double y1, double z1,
                  double x2, double y2, double z2, QREdgeStyle style)
                  : x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), style(style) {}
    virtual void acceptVisitor(sptr<BaseDrawMethodVisitor> &v);
    double x1, y1, z1, x2,y2,z2;
    QREdgeStyle style;
};

std::ostream& operator<<(std::ostream &os, const DrawablePoint &p);
std::ostream& operator<<(std::ostream &os, const DrawableEdge &e);

class BaseDrawMethodVisitor {
public:
    virtual void visitDrawPoint(DrawablePoint &p) = 0;
    virtual void visitDrawEdge(DrawableEdge &e) = 0;
private:
    sptr<Painter> painter;
};

class DrawMethodVisitor: public BaseDrawMethodVisitor {
public:
    DrawMethodVisitor(sptr<Painter> painter): painter(painter) {}
    void visitDrawPoint(DrawablePoint &p) {
        //cout << p <<'\n';
        painter->drawPoint(p.x, p.y, p.style);
    }
    void visitDrawEdge(DrawableEdge &e) {
        //cout << e;
        painter->drawEdge(e.x1,e.y1, e.x2, e.y2, e.style);
    }
private:
    sptr<Painter> painter;
};

class HideInvisibleDrawMethodVisitor: public BaseDrawMethodVisitor {
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
            sptr<Painter> painter): transformer(t), data(data), painter(painter) {}
    virtual void visitPoint3D(sptr<QRPoint3D> point);
    virtual void visitEdge3D(sptr<QREdge3D> edge);
    virtual void visitCamera3D(sptr<QRCamera3D> camera);

private:
    sptr<BaseTransformer3D> transformer;
    QRVector<sptr<DrawableData>> &data;
    sptr<Painter> painter;
};

class TransformVisitor: public QRVisitor {
public:
    TransformVisitor(sptr<BaseTransformer3D> t): transformer(t) {}
    virtual void visitPoint3D(sptr<QRPoint3D> point);
    virtual void visitEdge3D(sptr<QREdge3D> edge);
    virtual void visitCamera3D(sptr<QRCamera3D> camera);
    virtual void visitFrame3D(sptr<BaseFrame3D> frame);

protected:
    sptr<BaseTransformer3D> transformer;
};

class MoveTransformVisitor: public TransformVisitor {
public:
    MoveTransformVisitor(sptr<BaseTransformer3D> t): TransformVisitor(t) {}
    virtual void visitPoint3D(sptr<QRPoint3D> point);
    virtual void visitFrame3D(sptr<BaseFrame3D> frame);
protected:
    Vector3D frameBind;
};

class BindSetterVisitor: public QRVisitor {
public:
    BindSetterVisitor(const Vector3D &bind): bind(bind) {}
    virtual void visitPoint3D(sptr<QRPoint3D> point);
    virtual void visitEdge3D(sptr<QREdge3D> edge);
    virtual void visitCamera3D(sptr<QRCamera3D> camera);

protected:
    Vector3D bind;
};

class ScaleCameraVisitor: public TransformVisitor {
public:
    ScaleCameraVisitor(sptr<BaseTransformer3D> t): TransformVisitor(t) {}
    virtual void visitCamera3D(sptr<QRCamera3D> camera);
};

class SelectionVisitor: public QRVisitor {
public:
    SelectionVisitor(double x, double y, sptr<BaseTransformer3D> t): x(x), y(y), transformer(t) {}
    virtual void visitPoint3D(sptr<QRPoint3D> point);
    virtual void visitEdge3D(sptr<QREdge3D> edge);
    virtual void visitCamera3D(sptr<QRCamera3D> camera);

    bool is_selected;
protected:
    sptr<BaseTransformer3D> transformer;
    double x, y;
};

class SetColorVisitor: public QRVisitor {
public:
    SetColorVisitor(ColorKeeper keeper): keeper(keeper) {}
    virtual void visitPoint3D(sptr<QRPoint3D> point);
    virtual void visitEdge3D(sptr<QREdge3D> edge);
    virtual void visitCamera3D(sptr<QRCamera3D> camera);

    bool is_selected;
protected:
    ColorKeeper keeper;
};

#endif //BIG3DFLUFFY_VISITOR_H
