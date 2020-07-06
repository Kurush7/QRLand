//
// Created by kurush on 24.06.2020.
//

#ifndef BIG3DFLUFFY_MULTIPLEMEMENTOS_H
#define BIG3DFLUFFY_MULTIPLEMEMENTOS_H

#include "QRMemento.h"
#include "containers/QRContainers.h"

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
