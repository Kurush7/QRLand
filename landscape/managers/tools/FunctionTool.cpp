//
// Created by kurush on 16.08.2020.
//

#include "FunctionTool.h"

void FunctionTool::process() {
    double x, y = -(data.height+0.)/2.*data.worldStep;
    for(size_t i = 0; i < data.height; ++i) {
        x = -(data.width+0.)/2.*data.worldStep;
        for (size_t j = 0; j < data.width; ++j) {
            double z = exp(-(x * x + y * y) / 8) * (sin(x * x) + cos(y * y));
            (*data.hmap)[i][j] = z;
            x += data.worldStep;
        }
        y += data.worldStep;
    }
}