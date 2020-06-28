//
// Created by kurush on 24.06.2020.
//

#include "QRCamera3D.h"
#include "mementos/QRCamera3DMemento.h"

using namespace std;

uptr<QRMemento> QRCamera3D::save() {
    return uptr<QRMemento>(new Camera3DMemento(p));
}