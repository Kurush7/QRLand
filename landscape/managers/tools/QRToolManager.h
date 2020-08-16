//
// Created by kurush on 16.08.2020.
//

#ifndef BIG3DFLUFFY_QRTOOLMANAGER_H
#define BIG3DFLUFFY_QRTOOLMANAGER_H

#include "QRTool.h"
#include "containers/QRContainers.h"

enum ToolFrequency {
    freqAVERAGE = 1,
    freqRARE=10,
    freqUNIQUE = 100,
    freqLEGENDARY = 1000,
};

class QRToolManager {
public:
    void addTool(const sptr<QRTool> &tool, ToolFrequency freq) {
        tools.push_back({tool, freq});
    }
    const sptr<QRTool> & getTool() {
        if (tools.getSize() == 0)
            throw QRBadParamException(__FILE__, __LINE__, __TIME__,
                    "toolSet empty!");
        return tools[0].fst;    //todo
    }

private:
    QRVector<QRPair<sptr<QRTool>, ToolFrequency>> tools;
};


#endif //BIG3DFLUFFY_QRTOOLMANAGER_H
