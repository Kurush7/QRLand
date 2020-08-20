//
// Created by kurush on 20.08.2020.
//

#ifndef BIG3DFLUFFY_BFSWALK_H
#define BIG3DFLUFFY_BFSWALK_H

#include <functional>
#include <map>
#include <queue>

#include "QRTool.h"
#include "math/QRMath.h"

// frame of (0,0)-(w,h)
void BFSWalk(size_t w, size_t h, size_t startX, size_t startY,
        std::function<bool(size_t, size_t)> func);


#endif //BIG3DFLUFFY_BFSWALK_H
