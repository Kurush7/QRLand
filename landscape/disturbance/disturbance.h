//
// Created by kurush on 01.10.2020.
//

#ifndef BIG3DFLUFFY_DISTURBANCE_H
#define BIG3DFLUFFY_DISTURBANCE_H

#include "math/QRMath.h"
#include "math/QuickMath.h"
#include "containers/QRContainers.h"
#include "../managers/tools/QRTool.h"


class DisturbanceManager {
public:
    QRVectorIterator<sptr<QRTool>> getAll() {return tools.begin();}
    void add(sptr<QRTool> t) {tools.push_back(t);}

    void process() {
        //todo shuffle
        QRVector<sptr<QRTool>> new_tools;
        for (auto &tool: tools)
            if (tool->process())
                new_tools.push_back(tool);

        tools = new_tools;  // todo move not copy

    }

private:
    QRVector<sptr<QRTool>> tools;
};


#endif //BIG3DFLUFFY_DISTURBANCE_H
