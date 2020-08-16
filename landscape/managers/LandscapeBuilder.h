//
// Created by kurush on 15.08.2020.
//

#ifndef BIG3DFLUFFY_LANDSCAPEBUILDER_H
#define BIG3DFLUFFY_LANDSCAPEBUILDER_H

#include "../ROAM/RoamLandscape.h"
#include "../landscapeConfig.h"
#include "managers/tools/QRToolFabric.h"
#include "managers/tools/QRToolManager.h"

// todo avoid point matrix copies when building landscape

// here points are in range [-width/2, width/2][-height/2, height/2],
// but all calculations with height are made in [0, width][0, height]
class LandscapeBuilder {
public:
    LandscapeBuilder(size_t width, size_t height,
            size_t polyStep=1, double world_step=1);

    void setTools(QRVector<QRPair<ToolName, ToolFrequency>>);

    void process(int step_cnt);
    sptr<QRPolyModel3D> createLandscape();

private:
    sptr<QRPolyModel3D> landscape = nullptr;
    QRMatrix<double> heightMap;
    QRMatrix<sptr<QRPoint3D>> points;

    QRToolManager toolManager;

    size_t width, height;     // point width&height (points count, not polygons)
    size_t maxWidth, maxHeight;
    size_t step;
    double worldStep;  // step - polyStep (1,2,4 etc), x-y-Step - coordinate steps

    void clearHeightMap();
    void buildPoints();
    void updateHeightMap();
};

size_t squared(size_t x);
size_t squaredInc(size_t x);

#endif //BIG3DFLUFFY_LANDSCAPEBUILDER_H
