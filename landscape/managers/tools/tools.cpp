//
// Created by kurush on 04.10.2020.
//

#include "HillTool.h"
#include "../random_generator.cpp"

void HillTool::setToolData(const ToolData &dt) {
    data = dt;
    if (!inited) {
        inited=true;
        float maxH = maxHillHeightParam * data.worldStep;
        uniform_real_distribution<float> dist_hill(minHillHeightParam * data.worldStep, maxH);
        uniform_int_distribution<size_t> dist_w(0, data.width - 1);
        uniform_int_distribution<size_t> dist_h(0, data.height - 1);

        centerX = dist_w(default_generator);
        centerY = dist_h(default_generator);
        height = dist_hill(default_generator);
        give = data.worldStep;
    }
    else {
        // todo
        centerX *= 2;
        centerY *= 2;
    }
}