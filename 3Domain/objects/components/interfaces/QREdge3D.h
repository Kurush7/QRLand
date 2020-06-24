//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_QREDGE3D_H
#define BIG3DFLUFFY_QREDGE3D_H

#include "QRPoint3D.h"

class Edge3DMemento;

class QREdge3D: public QRObject3D {
public:
    QREdge3D(QREdgeStyle s = QREdgeStyle()): style(s) {p = std::shared_ptr<QREdge3D>(this, [](void *ptr){});}
    ~QREdge3D() {p.reset();}

    virtual void acceptVisitor(std::shared_ptr<QRVisitor> visitor) {visitor->visitEdge3D(p);}
    virtual std::unique_ptr<QRMemento> save();
    virtual bool isValid() const = 0;

    virtual const std::shared_ptr<QRPoint3D> getStart() const = 0;
    virtual void setStart(std::shared_ptr<QRPoint3D>) = 0;

    virtual const std::shared_ptr<QRPoint3D> getEnd() const = 0;
    virtual void setEnd(std::shared_ptr<QRPoint3D>) = 0;

    virtual const QREdgeStyle& getStyle() const {return style;}
    virtual void setStyle(const QREdgeStyle &s) {style = s;}

    virtual bool operator==(const QREdge3D &b) const = 0;
    virtual QREdge3D& operator=(const QREdge3D &p) { style = p.style;}

private:
    QREdgeStyle style;
    std::shared_ptr<QREdge3D> p;
};



#endif //BIG3DFLUFFY_QREDGE3D_H
