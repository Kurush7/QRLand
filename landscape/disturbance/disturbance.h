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
    int add(sptr<QRTool> t) {tools.push_back(t); active.push_back(true); return tools.getSize();}
    void setActive(int num, bool x) {active[num-1] = x;}
    void setIntensity(int num, float x) {tools[num-1]->setIntensity(x);}

    void process() {
        //todo shuffle
        QRVector<sptr<QRTool>> new_tools;
        QRVector<bool> new_active;
        for (int i = 0; i < tools.getSize(); ++i) {
            if (!active[i]) {
                new_tools.push_back(tools[i]);
                new_active.push_back(active[i]);
            }
            else if (tools[i]->process()) {
                new_tools.push_back(tools[i]);
                new_active.push_back(active[i]);
            }
        }

        tools = new_tools;  // todo move not copy
        active = new_active;
    }

private:
    QRVector<sptr<QRTool>> tools;
    QRVector<bool> active;
};


#endif //BIG3DFLUFFY_DISTURBANCE_H
