//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_MULTIPLEMEMENTOS_H
#define BIG3DFLUFFY_MULTIPLEMEMENTOS_H

#include "QRMemento.h"
#include "../composites/interfaces/QRComposite.h"

class GroupMemento: public QRMemento {
public:
    GroupMemento(sptr<QRComposite> obj): object(obj) {
        memes = obj->getObjects();
    }
    virtual void restore() {
        if (object.expired())
            throw QRBadPointerException(__FILE__, __LINE__, __TIME__, "Failed to create memento!");
        object.lock()->setObjects(memes);
    }
private:
    wptr<QRComposite> object;
    QRVector<sptr<QRObject>> memes;
};

class MementoAccumulator: public QRMemento {
public:
    MementoAccumulator() {}
    void add(sptr<QRMemento> m) {memes.push_back(m);}
    virtual void restore() {
        for (auto meme: memes)
            meme->restore();
        memes.clear();
    }
private:
    QRVector<sptr<QRMemento>> memes;
};

#endif //BIG3DFLUFFY_MULTIPLEMEMENTOS_H
