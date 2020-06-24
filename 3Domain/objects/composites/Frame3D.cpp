//
// Created by kurush on 31.05.2020.
//

#include "Frame3D.h"

uptr<QRMemento> BaseFrame3D::save() {return uptr<QRMemento>(new Frame3DMemento(p_frame));}

Frame3DMemento::Frame3DMemento(sptr<BaseFrame3D> obj): GroupMemento(obj), memes(new MementoAccumulator()) {
    for (auto x: obj->getObjects())
        memes->add(x->save());
}
void Frame3DMemento::restore() {
    GroupMemento::restore();
    memes->restore();
}