//
// Created by kurush on 31.05.2020.
//

#include "Frame3D.h"

std::unique_ptr<QRMemento> BaseFrame3D::save() {return unique_ptr<QRMemento>(new Frame3DMemento(p_frame));}

Frame3DMemento::Frame3DMemento(std::shared_ptr<BaseFrame3D> obj): GroupMemento(obj), memes(new MementoAccumulator()) {
    for (auto x: obj->getObjects())
        memes->add(x->save());
}
void Frame3DMemento::restore() {
    GroupMemento::restore();
    memes->restore();
}