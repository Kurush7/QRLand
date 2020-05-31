//
// Created by kurush on 31.05.2020.
//
#include "BaseComposite.h"

std::unique_ptr<Memento> BaseComposite::save() {return unique_ptr<Memento>(new GroupMemento(p));}