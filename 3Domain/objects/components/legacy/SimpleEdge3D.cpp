//
// Created by kurush on 24.06.2020.
//

#include "SimpleEdge3D.h"

SimpleEdge3D::SimpleEdge3D(sptr<QRPoint3D> start, sptr<QRPoint3D> end, QREdgeStyle s)
        : QREdge3D(s), start(start), end(end) {}

SimpleEdge3D::SimpleEdge3D(QRPoint3D &start, QRPoint3D &end, QREdgeStyle s)
        : QREdge3D(s), start(start.getPointer()), end(end.getPointer()) {}

const sptr<QRPoint3D> SimpleEdge3D::getStart() const {
    if (start.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to get edge's point!");
    }
    return start.lock();
}

const sptr<QRPoint3D> SimpleEdge3D::getEnd() const {
    if (end.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to get edge's point!");
    }
    return end.lock();
}

bool SimpleEdge3D::operator==(const QREdge3D &b) const {
    if (!isValid() || !b.isValid()) {
        time_t t = time(nullptr);
        throw QRBadPointException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to get edge's point!");
    }
    return start.lock() == b.getStart() && end.lock() == b.getEnd();
}