//
// Created by kurush on 30.05.2020.
//

#ifndef BIG3DFLUFFY_BASECOMPOSITE_H
#define BIG3DFLUFFY_BASECOMPOSITE_H

#include "../BaseObject.h"



class GroupMemento;
class BaseComposite: public BaseObject {
public:
    explicit BaseComposite(QRVector<std::shared_ptr<BaseObject>> &obj): objects(obj) {
        p = std::shared_ptr<BaseComposite>(this, [](void *ptr){});
    }
    virtual bool isComposite() {return true;}
    virtual void setSelected(bool x) {
        for (auto obj: objects) obj->setSelected(x);
        selected = x;
    }

    virtual const QRVector<std::shared_ptr<BaseObject>>& getObjects() {return objects;}
    virtual void setObjects(QRVector<std::shared_ptr<BaseObject>>&obj) {objects = obj;}

    virtual std::unique_ptr<Memento> save();

private:
    std::shared_ptr<BaseComposite> p;
protected:
    QRVector<std::shared_ptr<BaseObject>> objects;
};

class GroupMemento: public Memento {
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
    QRVector<std::shared_ptr<BaseObject>> memes;
};

class MementoAccumulator: public Memento {
public:
    MementoAccumulator() {}
    void add(std::shared_ptr<Memento> m) {memes.push_back(m);}
    virtual void restore() {
        for (auto meme: memes)
            meme->restore();
        memes.clear();
    }
private:
    QRVector<std::shared_ptr<Memento>> memes;
};

#endif //BIG3DFLUFFY_BASECOMPOSITE_H
