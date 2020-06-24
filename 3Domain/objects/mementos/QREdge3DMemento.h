//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_QREDGE3DMEMENTO_H
#define BIG3DFLUFFY_QREDGE3DMEMENTO_H

#include "QRMemento.h"
#include "../components/interfaces/QREdge3D.h"

class Edge3DMemento: public QRMemento {
public:
    Edge3DMemento(std::shared_ptr<QREdge3D>);
    virtual void restore();

private:
    std::weak_ptr<QREdge3D> object;
    std::weak_ptr<QRPoint3D> start, end;
    QREdgeStyle style;
};

#endif //BIG3DFLUFFY_QREDGE3DMEMENTO_H
