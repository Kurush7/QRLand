//
// Created by kurush on 24.06.2020.
//

#include "QRScene3D.h"
#include "mementos/QRSceneMemento.h"

using namespace std;

uptr<QRMemento> QRScene3D::save() {return uptr<QRMemento>(new QRSceneMemento(p));}