//
// Created by kurush on 16.08.2020.
//

#include "QRToolManager.h"

const sptr<QRTool>& QRToolManager::getTool() {
    if (tools.isEmpty())
        throw QRBadParamException(__FILE__, __LINE__, __TIME__,
                                  "toolSet empty!");

    int val = rand() % freq_sum;
    int i = 0, sum = 0;
    while (sum <= val)
        sum += frequences[i++];
    return tools[i-1];
}