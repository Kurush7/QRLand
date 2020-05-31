//
// Created by kurush on 30.05.2020.
//

#include "Edge3D.h"

using namespace std;


unique_ptr<Memento> BaseEdge3D::save() {
    return unique_ptr<Memento>(new Edge3DMemento(p));
}

Edge3DMemento::Edge3DMemento(shared_ptr<BaseEdge3D> p) {
    object = p;
    start = p->getStart();
    end = p->getEnd();
    style = p->getStyle();
}

void Edge3DMemento::restore() {
    if (object.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointerException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to create memento!");
    }
    if (start.expired() || end.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to create memento!");
    }
    shared_ptr<BaseEdge3D> p(object);
    p->setStart(start.lock());
    p->setEnd(end.lock());
    p->setStyle(style);
}

Edge3D::Edge3D(shared_ptr<BasePoint3D> start, shared_ptr<BasePoint3D> end, EdgeStyle s)
: BaseEdge3D(s), start(start), end(end) {}

Edge3D::Edge3D(BasePoint3D &start, BasePoint3D &end, EdgeStyle s)
:BaseEdge3D(s), start(start.getPointer()), end(end.getPointer()) {}

const std::shared_ptr<BasePoint3D> Edge3D::getStart() const {
    if (start.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to get edge's point!");
    }
    return start.lock();
}

const std::shared_ptr<BasePoint3D> Edge3D::getEnd() const {
    if (end.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to get edge's point!");
    }
    return end.lock();
}

bool Edge3D::operator==(const BaseEdge3D &b) const {
    if (!isValid() || !b.isValid()) {
        time_t t = time(nullptr);
        throw QRBadPointException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to get edge's point!");
    }
    return start.lock() == b.getStart() && end.lock() == b.getEnd();
}