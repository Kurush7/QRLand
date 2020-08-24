//
// Created by kurush on 16.08.2020.
//

#ifndef BIG3DFLUFFY_HILLTOOL_H
#define BIG3DFLUFFY_HILLTOOL_H

#include "QRTool.h"
#include "BFSWalk.h"

class HillTool: public QRTool {
public:
    virtual void process() {
        float maxH = maxHillHeightParam*data.worldStep;
        uniform_int_distribution<size_t> dist_w(0, data.width-1);
        uniform_int_distribution<size_t> dist_h(0, data.height-1);
        uniform_real_distribution<float> dist_hill(minHillHeightParam*data.worldStep, maxH);
        uniform_real_distribution<float> coef_d(minHillSteepCoef, minHillSteepCoef);

        size_t centerX = dist_w(generator);
        size_t centerY = dist_h(generator);
        float height = dist_hill(generator);

        QRMatrix<float> *local = data.hmap;
        float coef = -1 * coef_d(generator);
        auto f = [centerX, centerY, height, coef, local](size_t x, size_t y){
            float dist = sqrt((x-centerX)*(x-centerX) + (y-centerY)*(y-centerY));
            float h = height + dist*coef;
            (*local)[y][x] += h;
            return (h < QREPS);
        };

        BFSWalk(data.width, data.height, centerX, centerY, f);
    }

private:
    std::default_random_engine generator = std::default_random_engine();
};

class HillToolCreator: public QRToolCreator {
public:
    virtual uptr<QRTool> create() {return uptr<QRTool>(new HillTool());}
};



#endif //BIG3DFLUFFY_HILLTOOL_H
