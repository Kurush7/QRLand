//
// Created by kurush on 31.05.2020.
//
#include "BaseComposite.h"

uptr<QRMemento> BaseComposite::save() {return uptr<QRMemento>(new GroupMemento(p));}