//
// Created by kurush on 24.06.2020.
//

#include "QREdge3D.h"
#include "objects/mementos/QREdge3DMemento.h"

using namespace std;

uptr<QRMemento> QREdge3D::save() {
    return uptr<QRMemento>(new Edge3DMemento(p));
}