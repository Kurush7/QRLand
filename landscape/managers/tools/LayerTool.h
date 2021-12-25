//
// Created by kurush on 16.08.2020.
//

#ifndef BIG3DFLUFFY_LAYERTOOL_H
#define BIG3DFLUFFY_LAYERTOOL_H

#include "QRTool.h"

class LayerTool: public QRTool {
public:
    virtual bool process() {
        // not good - every time to create new generators - is it?
        uniform_int_distribution<size_t> dist_w(0, data.width-1);
        uniform_int_distribution<size_t> dist_h(0, data.height-1);

        size_t xl = dist_w(generator), xr = dist_w(generator);
        size_t yd = dist_h(generator), yu = dist_h(generator);
        if (xl > xr) swap(xl, xr);
        if (xr - xl > data.width/2) xr = xl + data.width/2;

        if (yd > yu) swap(yd, yu);
        if (yu - yd > data.height/2) yu = yd + data.height/2;

        for (size_t x = xl; x <= xr; ++x)
            for (size_t y = yd; y <= yu; ++y)
                (*data.hmap)[y][x] += data.worldStep * layerLevelMult;

        return false;
    }

private:
    std::default_random_engine generator = std::default_random_engine();
};

class LayerToolCreator: public QRToolCreator {
public:
    virtual uptr<QRTool> create() {return uptr<QRTool>(new LayerTool());}
};

#endif //BIG3DFLUFFY_LAYERTOOL_H
