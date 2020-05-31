//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_VISITOR_H
#define BIG3DFLUFFY_VISITOR_H


#include "../components/Point3D.h"
#include "../components/Edge3D.h"
#include "../components/Camera3D.h"
#include "../composites/Frame3D.h"
#include "../BaseObject.h"
#include "../../Painter.h"

class DrawMethodVisitor;
struct DrawableData {
    virtual void acceptVisitor(DrawMethodVisitor &v) = 0;
};
struct DrawablePoint: public DrawableData {
    DrawablePoint(double x, double y, double z, QRPointStyle style): x(x), y(y), z(z), style(style) {}
    virtual void acceptVisitor(DrawMethodVisitor &v);
    double x, y, z;
    QRPointStyle style;
};
struct DrawableEdge: public DrawableData {
    DrawableEdge(double x1, double y1, double z1,
                  double x2, double y2, double z2, QREdgeStyle style)
                  : x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), style(style) {}
    virtual void acceptVisitor(DrawMethodVisitor &v);
    double x1, y1, z1, x2,y2,z2;
    QREdgeStyle style;
};

std::ostream& operator<<(std::ostream &os, const DrawablePoint &p);
std::ostream& operator<<(std::ostream &os, const DrawableEdge &e);

class DrawMethodVisitor {
public:
    DrawMethodVisitor(std::shared_ptr<Painter> painter): painter(painter) {}
    void visitDrawPoint(DrawablePoint &p) {
        cout << p <<'\n';
        painter->drawPoint(p.x, p.y, p.style);
    }
    void visitDrawEdge(DrawableEdge &e) {
        cout << e;
        painter->drawEdge(e.x1,e.y1, e.x2, e.y2, e.style);
    }
private:
    std::shared_ptr<Painter> painter;
};


class DrawVisitor: public Visitor {
public:
    DrawVisitor(std::shared_ptr<BaseTransformer3D> t, QRVector<std::shared_ptr<DrawableData>> &data): transformer(t), data(data) {}
    virtual void visitPoint3D(std::shared_ptr<BaseQRPoint3D> point);
    virtual void visitEdge3D(std::shared_ptr<BaseEdge3D> edge);
    virtual void visitCamera3D(std::shared_ptr<BaseCamera3D> camera);
    virtual void visitFrame3D(std::shared_ptr<BaseFrame3D> frame);

private:
    std::shared_ptr<BaseTransformer3D> transformer;
    QRVector<std::shared_ptr<DrawableData>> &data;
};

#endif //BIG3DFLUFFY_VISITOR_H
