//
// Created by kurush on 02.08.2020.
//

#ifndef BIG3DFLUFFY_TIMETEST_H
#define BIG3DFLUFFY_TIMETEST_H

#include <chrono>
#include <functional>

using namespace std;
using namespace chrono;

double measureTime(std::function<void()> f);


#endif //BIG3DFLUFFY_TIMETEST_H
