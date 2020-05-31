//
// Created by Георгий Куликов on 29.04.2020.
//

#include "VectorBase.h"


VectorBase::VectorBase(size_t sz) {
    this->size = std::shared_ptr<size_t>(new size_t(0));
    if (!this->size)
        throw ErrorAlloc(__FILE__, __LINE__, "can't allocate memory", __DATE__);
}

VectorBase::~VectorBase() {}

size_t VectorBase::getSize() const {
    return *(this->size);
}

bool VectorBase::isEmpty() const {
    return *(this->size) == 0;
}

VectorBase::operator bool() const {
    return *(this->size) != 0;
}
