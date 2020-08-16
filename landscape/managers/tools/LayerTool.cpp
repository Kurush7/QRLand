//
// Created by kurush on 16.08.2020.
//

#include "LayerTool.h"

using namespace std;

void LayerTool::process() {
    uniform_int_distribution<size_t> dist_w(0, data.width-1);
    uniform_int_distribution<size_t> dist_h(0, data.height-1);

    size_t xl = dist_w(generator), xr = dist_w(generator);
    size_t yd = dist_h(generator), yu = dist_h(generator);
    if (xl > xr) swap(xl, xr);
    if (yd > yu) swap(yd, yu);

    for (size_t x = xl; x <= xr; ++x)
        for (size_t y = yd; y <= yu; ++y)
            (*data.hmap)[y][x] += data.worldStep;
}