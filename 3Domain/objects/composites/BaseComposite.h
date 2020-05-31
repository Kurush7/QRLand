//
// Created by kurush on 30.05.2020.
//

#ifndef BIG3DFLUFFY_BASECOMPOSITE_H
#define BIG3DFLUFFY_BASECOMPOSITE_H

#include "../BaseObject.h"


class GroupMemento: public Memento {
public:
    GroupMemento(QRVector<std::shared_ptr<BaseObject>> objects) {
        for (auto obj: objects)
            memes.push_back(obj->save());
    }
    virtual void restore() {
        for (auto meme: memes)
            meme->restore();
    }
private:
    QRVector<std::shared_ptr<Memento>> memes;
};


class BaseComposite: public BaseObject {
public:
    BaseComposite(QRVector<std::shared_ptr<BaseObject>> obj): objects(obj) {}
    virtual bool isComposite() {return true;}

    virtual std::unique_ptr<Memento> save() {return std::unique_ptr<Memento>(new GroupMemento(objects));}

protected:
    QRVector<std::shared_ptr<BaseObject>> objects;
};

#endif //BIG3DFLUFFY_BASECOMPOSITE_H
