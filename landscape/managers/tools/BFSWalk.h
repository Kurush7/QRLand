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
void BFSHillWalk(QRQueue<QRPair<int, int>> &start_q, int w, int h, QRMatrix<float> *hmap, float worldStep);
void BFSMountainWalk(QRQueue<QRPair<int, int>> &start_q, int w, int h, QRMatrix<float> *hmap, float worldStep);


#endif //BIG3DFLUFFY_BFSWALK_H
