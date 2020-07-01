//
// Created by kurush on 26.06.2020.
//

#include "QRPolygon3D.h"
#include "mementos/QRPolygon3DMemento.h"

uptr<QRMemento> QRPolygon3D::save() {
    return uptr<QRMemento>(new QRPolygon3DMemento(p));
}

std::ostream& operator<<(std::ostream &os, const sptr<QRPolygon3D> &p) {
    os << "<Polygon, " << p->getSize() << " pts: ";
    for (auto it = p->getPoints(); it; ++it)
        os << (*it)->getVector() << " ";
    os << ">";
}