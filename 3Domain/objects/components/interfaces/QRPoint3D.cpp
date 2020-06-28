//
// Created by kurush on 30.05.2020.
//

#include "QRPoint3D.h"
#include "mementos/QRPoint3DMemento.h"

using namespace std;

uptr<QRMemento> QRPoint3D::save() {
    return uptr<QRMemento>(new QRPoint3DMemento(p));
}

bool QRPoint3D::operator==(const QRPoint3D &b) const {
    return getVector() == b.getVector() &&
           getBind() == b.getBind() &&
           getStyle() == b.getStyle();
}