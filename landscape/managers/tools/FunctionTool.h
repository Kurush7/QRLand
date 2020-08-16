//
// Created by kurush on 16.08.2020.
//

#ifndef BIG3DFLUFFY_FUNCTIONTOOL_H
#define BIG3DFLUFFY_FUNCTIONTOOL_H

#include "QRTool.h"

class FunctionTool: public QRTool {
public:
    virtual void process();
};

class FunctionToolCreator: public QRToolCreator {
public:
    virtual uptr<QRTool> create() {return uptr<QRTool>(new FunctionTool());}
};



#endif //BIG3DFLUFFY_FUNCTIONTOOL_H
