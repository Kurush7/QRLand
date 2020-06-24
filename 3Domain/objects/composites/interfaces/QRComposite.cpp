//
// Created by kurush on 24.06.2020.
//

#include "QRComposite.h"

#include "mementos/MultipleMementos.h"

uptr<QRMemento> QRComposite::save() {return uptr<QRMemento>(new GroupMemento(p));}