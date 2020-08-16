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
        tools.push_back(tool);
        frequences.push_back(freq);
        freq_sum += freq;
    }
    const sptr<QRTool> & getTool();

private:
    QRVector<sptr<QRTool>> tools;
    QRVector<ToolFrequency> frequences;
    int freq_sum = 0;
};


#endif //BIG3DFLUFFY_QRTOOLMANAGER_H
