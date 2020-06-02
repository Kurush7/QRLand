//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_CARETAKER_H
#define BIG3DFLUFFY_CARETAKER_H

#include "../objects/QRObjects.h"
#include "../containers/QRContainers.h"

class BaseCareTaker {
public:
    virtual void cache(std::shared_ptr<Memento> mem) = 0;
    virtual bool isEmpty() = 0;
    virtual void restore() = 0;
};

class CareTaker: public BaseCareTaker {
public:
    virtual void cache(std::shared_ptr<Memento> mem) {mementos.push(mem);}
    virtual void restore() {mementos.pop()->restore();}
    virtual bool isEmpty() {return mementos.isEmpty();}
private:
    QRStack<std::shared_ptr<Memento>> mementos;
};


#endif //BIG3DFLUFFY_CARETAKER_H
