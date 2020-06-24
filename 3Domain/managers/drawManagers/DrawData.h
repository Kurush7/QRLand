//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_DRAWDATA_H
#define BIG3DFLUFFY_DRAWDATA_H

class QRDrawVisitor;
struct DrawableData {
    virtual void acceptVisitor(sptr<QRDrawVisitor> &v) = 0;
};
struct DrawablePoint: public DrawableData {
    DrawablePoint(double x, double y, double z, QRPointStyle style): x(x), y(y), z(z), style(style) {}
    virtual void acceptVisitor(sptr<QRDrawVisitor> &v);
    double x, y, z;
    QRPointStyle style;
};
struct DrawableEdge: public DrawableData {
    DrawableEdge(double x1, double y1, double z1,
                 double x2, double y2, double z2, QREdgeStyle style)
            : x1(x1), y1(y1), z1(z1), x2(x2), y2(y2), z2(z2), style(style) {}
    virtual void acceptVisitor(sptr<QRDrawVisitor> &v);
    double x1, y1, z1, x2,y2,z2;
    QREdgeStyle style;
};

#endif //BIG3DFLUFFY_DRAWDATA_H
