//
// Created by kurush on 16.08.2020.
//

#ifndef BIG3DFLUFFY_QRTOOL_H
#define BIG3DFLUFFY_QRTOOL_H

#include <random>

#include "containers/QRContainers.h"
#include "toolConfig.h"

struct ToolData {
    ToolData() = default;
    ToolData(QRMatrix<double> *hmap, size_t w, size_t h,
            double wS): hmap(hmap), width(w), height(h), worldStep(wS) {

    }

    QRMatrix<double> *hmap = nullptr;
    size_t width, height;     // point width&height (points count, not polygons)
    double worldStep;  // step - polyStep (1,2,4 etc), x-y-Step - coordinate steps
};

class QRTool {
public:
    virtual void process() = 0;
    virtual void setToolData(const ToolData &dt) {data = dt;}
protected:
    ToolData data;
};

class QRToolCreator {
public:
    virtual uptr<QRTool> create() = 0;
};

#endif //BIG3DFLUFFY_QRTOOL_H
