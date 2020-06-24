//
// Created by kurush on 30.05.2020.
//

#ifndef BIG3DFLUFFY_BASECOMPOSITE_H
#define BIG3DFLUFFY_BASECOMPOSITE_H

#include "objects/QRObject.h"



class GroupMemento;
class BaseComposite: public QRObject {
public:
    explicit BaseComposite(QRVector<sptr<QRObject>> &obj): objects(obj) {
        p = sptr<BaseComposite>(this, [](void *ptr){});
    }
    virtual bool isComposite() {return true;}
    virtual void setSelected(bool x) {
        for (auto obj: objects) obj->setSelected(x);
        selected = x;
    }

    // todo iterators only!
    virtual const QRVector<sptr<QRObject>>& getObjects() {return objects;}
    virtual void setObjects(QRVector<sptr<QRObject>>&obj) { objects = obj;}

    virtual uptr<QRMemento> save();

private:
    sptr<BaseComposite> p;
protected:
    QRVector<sptr<QRObject>> objects;
};

class GroupMemento: public QRMemento {
public:
    GroupMemento(sptr<BaseComposite> obj): object(obj) {
        memes = obj->getObjects();
    }
    virtual void restore() {
        if (object.expired())
            throw QRBadPointerException(__FILE__, __LINE__, __TIME__, "Failed to create memento!");
        object.lock()->setObjects(memes);
    }
private:
    wptr<BaseComposite> object;
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

#endif //BIG3DFLUFFY_BASECOMPOSITE_H
