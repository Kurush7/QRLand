//
// Created by kurush on 15.08.2020.
//

#include "LandscapeBuilder.h"

size_t squared(size_t x) {
    size_t a = 1;
    while (a < x-1) a *= 2;
    if (a > x) a /= 2;
    return a;
}
size_t squaredInc(size_t x) {
    return squared(x) + 1;
}

LandscapeBuilder::LandscapeBuilder(size_t _width, size_t _height,
        size_t polyStep, double world_step)
: heightMap(squared(_width)/squared(polyStep)+1,
         squared(_width)/squared(polyStep)+1),
points(squared(_width)/squared(polyStep)+1,
squared(_width)/squared(polyStep)+1),
  plateManager(squaredInc(_width)*world_step-world_step,
               squaredInc(_height)*world_step-world_step)
{

    maxWidth = squaredInc(_width), maxHeight = squaredInc(_height);
    step = squared(polyStep);
    width = squared(_width) / step + 1,
    height = squared(_width) / step + 1;

    worldStep = world_step;

    clearHeightMap();
    buildPoints();
}

void LandscapeBuilder::setTools(QRVector<QRPair<ToolName, ToolFrequency>> toolSet) {
    ToolData data(&heightMap, width, height, worldStep);

    auto toolFabric = initToolFabric();
    sptr<QRTool> tool;
    for (auto &item: toolSet) {
        tool = toolFabric.create(item.fst);
        tool->setToolData(data);
        toolManager.addTool(tool, item.snd);
    }
    // todo setHMap and others for new tool
}

void LandscapeBuilder::clearHeightMap() {
    for(size_t i = 0; i < height; ++i)
        for(size_t j = 0; j < width; ++j)
            heightMap[i][j] = 0;
}

void LandscapeBuilder::buildPoints() {
    double x, y = -(height+0.)/2.*worldStep;
    for(size_t i = 0; i < height; ++i) {
        x = -(width+0.)/2.*worldStep;
        for (size_t j = 0; j < width; ++j) {
            double z = heightMap[i][j];
            points[i][j] = sptr<QRPoint3D>(new Point3D(x, y, z));
            x += worldStep;
        }
        y += worldStep;
    }
}

void LandscapeBuilder::process(int step_cnt) {
    for (int i = 0; i < step_cnt; ++i)
        toolManager.getTool()->process();

    updateHeightMap();
}

void LandscapeBuilder::useTool(ToolName name) {
    // todo ineffective
    ToolData data(&heightMap, width, height, worldStep);
    auto toolFabric = initToolFabric();
    sptr<QRTool> tool = toolFabric.create(name);
    tool->setToolData(data);
    tool->process();
    updateHeightMap();
}

sptr<QRPolyModel3D> LandscapeBuilder::createLandscape() {
    if (!landscape) {
        // todo consider *2 (square is 2 triangles, so...)
        if (width*height > maxNoLODPolygonCnt)
            landscape = sptr<QRPolyModel3D>(new RoamLandscape(points));
        else
            landscape = sptr<QRPolyModel3D>(new QRLandscapeSurface(points));
    }
    return landscape;
}

void LandscapeBuilder::updateHeightMap() {
    Vector3D v;
    for(size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            v = points[i][j]->getVector();
            v[2] = heightMap[i][j];
            points[i][j]->setVector(v);
        }
    }
}