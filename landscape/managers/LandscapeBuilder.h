//
// Created by kurush on 15.08.2020.
//

#ifndef BIG3DFLUFFY_LANDSCAPEBUILDER_H
#define BIG3DFLUFFY_LANDSCAPEBUILDER_H

#include "../ROAM/RoamLandscape.h"
#include "../water/WaterManager.h"
#include "../climate/ClimateManager.h"


#include "../landscapeConfig.h"
#include "managers/tools/QRToolFabric.h"
#include "managers/tools/QRToolManager.h"

#include "../disturbance/disturbance.h"

#include "PlateManager.h"

// todo avoid point matrix copies when building landscape

// todo when updating worldStep and others (polygons, points)...
//  inform waterManager!!!

// here points are in range [-width/2, width/2][-height/2, height/2],
// but all calculations with height are made in [0, width][0, height]
class LandscapeBuilder {
public:
    LandscapeBuilder(size_t width, size_t height, double world_step=1);

    void setTools(QRVector<QRPair<ToolName, ToolFrequency>>);

    void process(int step_cnt=1);
    void useTool(ToolName);

    void scaleGrid();

    sptr<RoamLandscape> createLandscape();

    const QRMatrix<float>& getHeightMap() {return heightMap;}
    double getWorldStep() {return worldStep;}

    bool activateWaterManager() {
        if (landscape) {
            // todo method not needed
            //waterManager->setPolygons(lowest_polygons.begin());
        }
        return bool(landscape);
    }

    PlateManager plateManager;
    sptr<WaterManager> waterManager;
    sptr<ClimateManager> climateManager;
    DisturbanceManager disturbManager;
    QRToolManager toolManager;
private:
    sptr<RoamLandscape> landscape = nullptr;
    QRVector<sptr<QRPolygon3D>> lowest_polygons;
    QRMatrix<float> heightMap;
    QRMatrix<sptr<QRPoint3D>> points;

    ToolData toolData;

    std::default_random_engine generator = std::default_random_engine();

    size_t width, height;     // point width&height (points count, not polygons)
    double worldStep;  // world-grid steps for each axis

    void clearHeightMap();
    void buildPoints();
    void updateHeightMap();
};

size_t squared(size_t x);
size_t squaredInc(size_t x);

#endif //BIG3DFLUFFY_LANDSCAPEBUILDER_H
