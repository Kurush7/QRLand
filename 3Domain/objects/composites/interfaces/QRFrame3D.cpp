//
// Created by kurush on 24.06.2020.
//

#include "QRFrame3D.h"
#include "../../mementos/QRFrame3DMemento.h"

uptr<QRMemento> QRFrame3D::save() {return uptr<QRMemento>(new QRFrame3DMemento(p_frame));}