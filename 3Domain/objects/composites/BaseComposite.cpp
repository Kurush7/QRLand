//
// Created by kurush on 31.05.2020.
//
#include "BaseComposite.h"

std::unique_ptr<QRMemento> BaseComposite::save() {return unique_ptr<QRMemento>(new GroupMemento(p));}