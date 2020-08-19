//
// Created by kurush on 16.08.2020.
//

#include "QRToolFabric.h"

using namespace std;

bool QRToolFabric::registerTool(ToolName name, const sptr<QRToolCreator> &creator) {
    if (creators.find(name) != creators.end()) return false;
    creators[name] = creator;
    return true;
}

uptr<QRTool> QRToolFabric::create(ToolName name) {
    if (creators.find(name) == creators.end())
        throw QRBadParamException(__FILE__, __LINE__, __TIME__,
                "tool-creator not found");

    return creators[name]->create();
}

QRToolFabric initToolFabric() {
    QRToolFabric manager;
    manager.registerTool(LayerTool,
            sptr<QRToolCreator>(new LayerToolCreator));
    manager.registerTool(FunctionTool,
            sptr<QRToolCreator>(new FunctionToolCreator));
    manager.registerTool(HillTool,
                         sptr<QRToolCreator>(new HillToolCreator));
    return manager;
}