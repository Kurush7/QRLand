//
// Created by kurush on 28.06.2020.
//

#ifndef BIG3DFLUFFY_GLOBALDEFINES_H
#define BIG3DFLUFFY_GLOBALDEFINES_H

#include <memory>
#include <chrono>
#include <functional>

// used for time measuring

#define timeStampCount 64

using timeStamp = std::chrono::system_clock::time_point;
static timeStamp startTime, startTimes[timeStampCount], endTimes[timeStampCount];
static double timesAccumulator[timeStampCount] = {0};

#define resetAccumulators for(int i = 0; i < timeStampCount; ++i) timesAccumulator[i] = 0

#define startMeasureTime startTime = (std::chrono::system_clock::now())
#define endMeasureTime ((std::chrono::system_clock::now() - startTime).count() / 1e6)

#define startMeasureTimeStamp(X) startTimes[X] = (std::chrono::system_clock::now())
#define endMeasureTimeValue(X) ((std::chrono::system_clock::now() - startTimes[X]).count() / 1e6)
#define endMeasureTimeIncrement(X) timesAccumulator[X] += ((std::chrono::system_clock::now() - startTimes[X]).count() / 1e6)

#define measureTimeStamp(X) ((endTimes[X] - startTimes[X]).count() / 1e6)
#define measureTimeAccumulator(X) (timesAccumulator[X])


#define wptr std::weak_ptr
#define sptr std::shared_ptr
#define uptr std::unique_ptr

#define qrfor(type, var, l, r) for(type var = l; var < r; ++var)



#endif //BIG3DFLUFFY_GLOBALDEFINES_H
