//
// Created by kurush on 30.05.2020.
//

#ifndef BIG3DFLUFFY_BASECOMPOSITE_H
#define BIG3DFLUFFY_BASECOMPOSITE_H

#include "objects/QRObject.h"



class GroupMemento;
class BaseComposite: public QRObject {
public:
    explicit BaseComposite(QRVector<std::shared_ptr<QRObject>> &obj): objects(obj) {
        p = std::shared_ptr<BaseComposite>(this, [](void *ptr){});
    }
    virtual bool isComposite() {return true;}
    virtual void setSelected(bool x) {
        for (auto obj: objects) obj->setSelected(x);
        selected = x;
    }

    // todo iterators only!
    virtual const QRVector<std::shared_ptr<QRObject>>& getObjects() {return objects;}
    virtual void setObjects(QRVector<std::shared_ptr<QRObject>>&obj) { objects = obj;}

    virtual std::unique_ptr<QRMemento> save();

private:
    std::shared_ptr<BaseComposite> p;
protected:
    QRVector<std::shared_ptr<QRObject>> objects;
};

class GroupMemento: public QRMemento {
public:
    GroupMemento(std::shared_ptr<BaseComposite> obj): object(obj) {
        memes = obj->getObjects();
    }
    virtual void restore() {
        if (object.expired())
            throw QRBadPointerException(__FILE__, __LINE__, __TIME__, "Failed to create memento!");
        object.lock()->setObjects(memes);
    }
private:
    std::weak_ptr<BaseComposite> object;
    QRVector<std::shared_ptr<QRObject>> memes;
};

class MementoAccumulator: public QRMemento {
public:
    MementoAccumulator() {}
    void add(std::shared_ptr<QRMemento> m) {memes.push_back(m);}
    virtual void restore() {
        for (auto meme: memes)
            meme->restore();
        memes.clear();
    }
private:
    QRVector<std::shared_ptr<QRMemento>> memes;
};

#endif //BIG3DFLUFFY_BASECOMPOSITE_H
