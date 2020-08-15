//
// Created by kurush on 01.06.2020.
//

#ifndef BIG3DFLUFFY_QRCONSTANTS_H
#define BIG3DFLUFFY_QRCONSTANTS_H

#include <thread>

const int RENDER_THREAD_CNT = std::thread::hardware_concurrency();

const float QREPS = 1e-5;
const float QRSMALL_EPS = 1e-9;
const float QREPS_MINI = 1e-3;
const float QRINF = 1e9;

const float SELECTION_ERROR = 1.05;    // for selection of frame model on click

const float BUTTON_REPRESS_TIME_MSEC = 1000/60;

const int MAX_LIGHT_CNT = 10;
const int MAX_POINT_CNT = 1e6;

const float SCENE_DIST = 10.;
const float PROJECTIVE_COEF = 1./30.;




#endif //BIG3DFLUFFY_QRCONSTANTS_H
