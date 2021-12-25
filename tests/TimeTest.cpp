//
// Created by kurush on 02.08.2020.
//

#include "TimeTest.h"

using namespace std;
using namespace chrono;

double measureTime(std::function<void()> f) {
    system_clock::time_point start = system_clock::now();
    f();
    system_clock::time_point end = system_clock::now();
    double time = (end - start).count();    // nanosecs
    return time / 1e6;  // milisecs
}