//
// Created by kurush on 31.05.2020.
//

#ifndef BIG3DFLUFFY_SIMPLEFRAME3D_H
#define BIG3DFLUFFY_SIMPLEFRAME3D_H

#include "../interfaces/QRFrame3D.h"

class SimpleFrame3D: public QRFrame3D {
public:
    explicit SimpleFrame3D(QRVector<sptr<QRObject>> obj): QRFrame3D(obj) {}

    virtual ObjectIterator begin() {return objects.begin();}
    virtual ObjectIterator end() { return objects.end();}
};


#endif //BIG3DFLUFFY_SIMPLEFRAME3D_H
