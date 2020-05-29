//
// Created by Георгий Куликов on 03.05.2020.
//

#ifndef LAB2_ITERATOR_H
#define LAB2_ITERATOR_H


#include <cstdlib>
#include <memory>

class Iterator {
public:
    explicit Iterator(size_t sz = 0);
    virtual ~Iterator() = default;
protected:
    std::weak_ptr<size_t> size;
    size_t index = 0;
};


#endif //LAB2_ITERATOR_H
