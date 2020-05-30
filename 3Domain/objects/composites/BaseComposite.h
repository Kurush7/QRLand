//
// Created by kurush on 30.05.2020.
//

#ifndef BIG3DFLUFFY_BASECOMPOSITE_H
#define BIG3DFLUFFY_BASECOMPOSITE_H

#include "../BaseObject.h"

class BaseComposite: public BaseObject {
public:
    virtual bool isComposite() {return true;}
    virtual ObjectIterator begin() {return vector.begin();}
    virtual ObjectIterator end() {return vector.end();}
private:
    QRVector<shared_ptr<BaseObject>> vector;
};


#endif //BIG3DFLUFFY_BASECOMPOSITE_H
