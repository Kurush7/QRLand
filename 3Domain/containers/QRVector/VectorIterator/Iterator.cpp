//
// Created by Георгий Куликов on 03.05.2020.
//

#include "Iterator.h"

Iterator::Iterator(size_t sz) {
    this->size = std::shared_ptr<size_t>(new size_t(sz));
}
