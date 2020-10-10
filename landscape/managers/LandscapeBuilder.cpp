//
// Created by kurush on 15.08.2020.
//

#include "LandscapeBuilder.h"
#include "random_generator.h"

size_t squared(size_t x) {
    size_t a = 1;
    while (a < x-1) a *= 2;
    if (a > x) a /= 2;
    return a;
}
size_t squaredInc(size_t x) {
    return squared(x) + 1;
}

LandscapeBuilder::LandscapeBuilder(size_t _width, size_t _height, double world_step):
heightMap(squaredInc(_width),
          squaredInc(_width)),
points(squaredInc(_width),
       squaredInc(_width)),
plateManager(squaredInc(_width)*world_step,
               squaredInc(_height)*world_step)
{
    width = squaredInc(_width),
    height = squaredInc(_width);

    worldStep = world_step;

    clearHeightMap();
    buildPoints();

    // this must go when map and points are ready!
    waterManager = sptr<WaterManager>(new WaterManager(heightMap, points));

    toolData = ToolData(&heightMap, width, height, worldStep,
                        plateManager.getPlates(), plateManager.getMove());
}

void LandscapeBuilder::setTools(QRVector<QRPair<ToolName, ToolFrequency>> toolSet) {
    auto toolFabric = initToolFabric();
    sptr<QRTool> tool;
    for (auto &item: toolSet) {
        tool = toolFabric.create(item.fst);
        tool->setToolData(toolData);
        toolManager.addTool(tool, item.snd);
        disturbManager.add(tool);
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
        disturbManager.process();
    updateHeightMap();
}

void LandscapeBuilder::useTool(ToolName name) {
    auto toolFabric = initToolFabric();
    sptr<QRTool> tool = toolFabric.create(name);
    tool->setToolData(toolData);
    tool->process();
    updateHeightMap();
}

sptr<QRPolyModel3D> LandscapeBuilder::createLandscape() {
    landscape.reset();
    landscape = sptr<QRPolyModel3D>(new RoamLandscape(points));
    lowest_polygons.clear();
    landscape->addMaxDetailPolygons(lowest_polygons);
    waterManager->setPolygons(lowest_polygons);
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
    // todo update polygons
    if (!landscape) return;
    QRVector<sptr<QRPolygon3D>> polys;
    landscape->addAllPolygons(polys);
    auto pts = landscape->getPurePoints();
    for (size_t i = 0; i < polys.getSize(); ++i)
        polys[i]->updateNormalIndex(pts);
}

void LandscapeBuilder::scaleGrid() {
    // prepare
    auto newW = width*2 - 1;
    auto newH = height*2 - 1;
    auto newHmap = QRMatrix<float>(newW, newH);

    for (int i = 0, oldi=0; i < newH; i += 2, oldi++)
        for (int j = 0, oldj=0; j < newW; j+= 2, oldj++)
            newHmap[i][j] = heightMap[oldi][oldj];


    uniform_real_distribution<float> shift(-worldStep*diamondSquareRandomCoef, worldStep*diamondSquareRandomCoef);
    // diamond square
    for (int i = 1; i < newH; i += 2)
        for (int j = 1; j < newW; j += 2) {
            newHmap[i][j] = (newHmap[i-1][j - 1] + newHmap[i-1][j + 1] + newHmap[i+1][j-1] + newHmap[i+1][j+1]) / 4;
            newHmap[i][j] += shift(default_generator);
        }

    float tmp, cnt;
    for (int i = 0; i < newH; i++)
        for (int j = i%2? 0:1; j < newW; j+=2) {
            tmp = 0, cnt=0;
            if (j > 0) tmp += newHmap[i][j - 1], cnt++;
            if (j < newW-1) tmp += newHmap[i][j + 1], cnt++;
            if (i > 0) tmp += newHmap[i-1][j], cnt++;
            if (i < newH-1) tmp += newHmap[i+1][j], cnt++;
            newHmap[i][j] = tmp / cnt;
            newHmap[i][j] += shift(default_generator);
        }

    // update
    width = newW, height = newH;
    worldStep /= 2;
    heightMap = newHmap;

    points.resize(newW, newH);
    buildPoints();
    //plateManager.scalePlates();
    toolData = ToolData(&heightMap, width, height, worldStep,
                        plateManager.getPlates(), plateManager.getMove());

    waterManager->updateMatrices(heightMap, points);
    //lowest_polygons.clear();
    //landscape->addMaxDetailPolygons(lowest_polygons);
    //waterManager->setPolygons(lowest_polygons.begin());


    for (auto it = disturbManager.getAll(); it; ++it)
        it->get()->setToolData(toolData);
    //for (auto it = toolManager.getAllTools(); it; ++it)   // todo this is deprecated
    //    it->get()->setToolData(toolData);
}