//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_QREDGE3D_H
#define BIG3DFLUFFY_QREDGE3D_H

#include "QRPoint3D.h"

// todo: simple impl-n of an Edge keeps weak_ptrs for points..... probably shared is a better choice


class QREdge3D: public QRObject3D {
public:
    QREdge3D(QREdgeStyle s = QREdgeStyle()): style(s) {p = sptr<QREdge3D>(this, [](void *ptr){});}
    ~QREdge3D() {p.reset();}

    virtual void acceptVisitor(const sptr<QRVisitor>& visitor) {visitor->visitEdge3D(p);}
    virtual uptr<QRMemento> save();
    virtual bool isValid() const = 0;

    virtual const sptr<QRPoint3D> getStart() const = 0;
    virtual void setStart(sptr<QRPoint3D>) = 0;

    virtual const sptr<QRPoint3D> getEnd() const = 0;
    virtual void setEnd(sptr<QRPoint3D>) = 0;

    virtual const QREdgeStyle& getStyle() const {return style;}
    virtual void setStyle(const QREdgeStyle &s) {style = s;}

    virtual bool operator==(const QREdge3D &b) const = 0;
    virtual QREdge3D& operator=(const QREdge3D &p) { style = p.style;}

private:
    QREdgeStyle style;
    sptr<QREdge3D> p;
};



#endif //BIG3DFLUFFY_QREDGE3D_H
