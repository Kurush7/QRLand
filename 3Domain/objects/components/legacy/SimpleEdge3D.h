//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_SIMPLEEDGE3D_H
#define BIG3DFLUFFY_SIMPLEEDGE3D_H

#include "../interfaces/QREdge3D.h"

class SimpleEdge3D: public QREdge3D {
public:
    SimpleEdge3D(std::shared_ptr<QRPoint3D> start, std::shared_ptr<QRPoint3D> end, QREdgeStyle s = QREdgeStyle());
    SimpleEdge3D(QRPoint3D &start, QRPoint3D &end, QREdgeStyle s = QREdgeStyle());

    virtual bool isValid() const {return !start.expired() && !end.expired();}

    virtual const std::shared_ptr<QRPoint3D> getStart() const;
    virtual void setStart(std::shared_ptr<QRPoint3D> p) { start = p;}

    virtual const std::shared_ptr<QRPoint3D> getEnd() const;
    virtual void setEnd(std::shared_ptr<QRPoint3D> p) { end = p;}

    virtual bool operator==(const QREdge3D &b) const;

private:
    std::weak_ptr<QRPoint3D> start, end;
};



#endif //BIG3DFLUFFY_SIMPLEEDGE3D_H
