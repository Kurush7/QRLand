//
// Created by kurush on 28.06.2020.
//

#include "QRPolyScene3D.h"
#include "../../mementos/QRPolySceneMemento.h"

uptr<QRMemento> QRPolyScene3D::save() {return uptr<QRMemento>(new QRPolySceneMemento(p));}