//
// Created by kurush on 16.08.2020.
//

#ifndef BIG3DFLUFFY_HILLTOOL_H
#define BIG3DFLUFFY_HILLTOOL_H

#include "QRTool.h"

class HillTool: public QRTool {
public:
    virtual void process() {
        // todo hills must be continuous and less 'пупырки'
        float maxH = maxHillHeightParam*data.worldStep;
        uniform_int_distribution<size_t> dist_w(0, data.width-1);
        uniform_int_distribution<size_t> dist_h(0, data.height-1);
        uniform_real_distribution<float> dist_hill(minHillHeightParam*data.worldStep,
                                                   maxH);

        centerX = dist_w(generator);
        centerY = dist_h(generator);
        height = dist_hill(generator);

        uniform_int_distribution<size_t> dist_r(1 - maxHillRadius*(maxH-height)/maxH, maxHillRadius);
        radius = dist_r(generator);

        for (size_t x = max((size_t)0, centerX-radius); x <= min(centerX+radius, data.height); ++x)
            for (size_t y = max((size_t)0, centerY-radius); y <= min(centerY+radius, data.height); ++y)
                (*data.hmap)[y][x] += hillHeight(x,y);
    }

private:
    float hillHeight(size_t x, size_t y) {
        float dist = sqrt((x-centerX)*(x-centerX) + (y-centerY)*(y-centerY)+0.)*data.worldStep;
        float maxDist = radius*data.worldStep*sqrt(2);
        dist = dist * M_PI_2 / maxDist;
        float h = cos(dist)*height;
        return h;
    }
    size_t centerX, centerY;
    float height;
    size_t radius;

    std::default_random_engine generator = std::default_random_engine();
};

class HillToolCreator: public QRToolCreator {
public:
    virtual uptr<QRTool> create() {return uptr<QRTool>(new HillTool());}
};



#endif //BIG3DFLUFFY_HILLTOOL_H
