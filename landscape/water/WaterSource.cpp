//
// Created by kurush on 10.10.2020.
//

#include "WaterSource.h"
#include "../random_generator.h"

void  RainWaterSource::use(float dt) {
    size_t w, h;
    for(int k = 0; k < dropCnt; ++k) {
        w = dist_w(default_generator);
        h = dist_h(default_generator);
        for (int i = -dropRad+1; i < dropRad; ++i)
            for (int j = -dropRad+1; j < dropRad; ++j)
                if (w+i < width && w+i >= 0 && h+j<height && h+j >= 0)
                    waterLevel[h+j][w+i] += dropIntensity;
    }
}