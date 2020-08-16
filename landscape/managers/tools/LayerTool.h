//
// Created by kurush on 16.08.2020.
//

#ifndef BIG3DFLUFFY_LAYERTOOL_H
#define BIG3DFLUFFY_LAYERTOOL_H

#include "QRTool.h"

class LayerTool: public QRTool {
public:
    virtual void process();

private:
    std::default_random_engine generator = std::default_random_engine();
};

class LayerToolCreator: public QRToolCreator {
public:
    virtual uptr<QRTool> create() {return uptr<QRTool>(new LayerTool());}
};

#endif //BIG3DFLUFFY_LAYERTOOL_H
