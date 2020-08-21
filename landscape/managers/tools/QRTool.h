//
// Created by kurush on 16.08.2020.
//

#ifndef BIG3DFLUFFY_QRTOOL_H
#define BIG3DFLUFFY_QRTOOL_H

#include <random>
#include <functional>

#include "containers/QRContainers.h"
#include "math/QRMath.h"
#include "2Domain.h"
#include "toolConfig.h"

struct ToolData {
    ToolData() = default;
    ToolData(QRMatrix<double> *hmap, size_t w, size_t h,
            double wS, const QRVectorIterator<sptr<QRFrame2D>> &plates,
            const QRVectorIterator<Vector3D> &move): hmap(hmap), width(w), height(h), worldStep(wS),
            plates(plates), moveVectors(move) {

    }

    QRVector<sptr<QRFrame2D>> plates;
    QRVector<Vector3D> moveVectors;

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
