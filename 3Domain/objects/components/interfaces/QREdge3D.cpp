//
// Created by kurush on 24.06.2020.
//

#include "QREdge3D.h"
#include "mementos/QREdge3DMemento.h"

using namespace std;

unique_ptr<QRMemento> QREdge3D::save() {
    return unique_ptr<QRMemento>(new Edge3DMemento(p));
}