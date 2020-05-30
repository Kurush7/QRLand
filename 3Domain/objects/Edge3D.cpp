//
// Created by kurush on 30.05.2020.
//

#include "Edge3D.h"

using namespace std;


unique_ptr<Memento> BaseEdge3D::save() {
    return unique_ptr<Memento>(new Edge3DMemento(p));
}

Edge3DMemento::Edge3DMemento(weak_ptr<BaseEdge3D> wp) {
    if (wp.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointerException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to create memento!");
    }

    object = wp;
    shared_ptr<BaseEdge3D> p(wp);
    start = p->getStart();
    end = p->getEnd();
    style = p->getStyle();
}

void Edge3DMemento::restore() {
    if (object.expired()) {
        time_t t = time(nullptr);
        throw QRBadPointerException(__FILE__, __LINE__, asctime(localtime(&t)), "Failed to create memento!");
    }
    shared_ptr<BaseEdge3D> p(object);
    p->setStart(start);
    p->setEnd(end);
    p->setStyle(style);
}

Edge3D::Edge3D(weak_ptr<BasePoint3D> start, weak_ptr<BasePoint3D> end, EdgeStyle s)
: BaseEdge3D(s), start(start), end(end) {}

Edge3D::Edge3D(BasePoint3D &start, BasePoint3D &end, EdgeStyle s)
:BaseEdge3D(s), start(start.getPointer()), end(end.getPointer()) {}