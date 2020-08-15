//
// Created by kurush on 11.08.2020.
//

#include "RoamLandscape.h"
#include "RoamTree.cpp"

#include <sstream>

RoamLandscape::RoamLandscape(const QRMatrix<sptr<QRPoint3D>> &points): points(points) {
    startMeasureTime;
    size_t size = points.rowCnt();
    dimFrameCnt = (size-1) / (FrameSize - 1);
    lowestPolygonCnt = (size-1)*(size-1);

    frames = new Frame[dimFrameCnt*dimFrameCnt];

    size_t fsize = FrameSize-1;
    size_t index = 0;
    for (size_t i = 0; i < dimFrameCnt; ++i)
        for (size_t j = 0; j < dimFrameCnt; ++j) {
            Frame f(points, j * fsize, (j + 1) * fsize,
                    i * fsize, (i + 1) * fsize, links);
            frames[index].left = f.left;
            frames[index].right = f.right;
            frames[index].center = f.center;
            frames[index].radius = f.radius;
            frames[index].isVisible = f.isVisible;
            index++;
            f.left = nullptr;
            f.right = nullptr;
        }
    cout << "ROAM Landscaped formed in: " << endMeasureTime << "ms\n\n";
}

void RoamLandscape::updateCamera(const sptr<QRCamera3D> &camera, string *info) {
    size_t allSize = dimFrameCnt*dimFrameCnt;
    double d = camera->getScreenDistance();
    RoamUpdateConstant = maxPixelError*maxPixelError /d/d /pixelsPerUnit/pixelsPerUnit;
    auto c = camera->getWorldOriginCoord();
    CameraCoordsX = c[0], CameraCoordsY = c[1], CameraCoordsZ = c[2];
    
    size_t skippedFrames = 0;
    for (size_t i = 0; i < allSize; ++i)
        skippedFrames += !frames[i].updateCamera(camera);

    if (info) {
        ostringstream s;
        s << "POLYGON INFO:\n\tmodel's initial polygon count: " << lowestPolygonCnt;
        s << "\n\tframes cnt: " << allSize;
        s << "\n\tskipped frames: " << skippedFrames;
        s << " (polygons gone: " << skippedFrames*(FrameSize-1)*(FrameSize-1) << ")\n";
        *info = *info + s.str();
    }
}

void RoamLandscape::addPolygons(QRVector<sptr<QRPolygon3D>> &polygons, string *info) {
    lastPolyCount = polygons.getSize();
    size_t allSize = dimFrameCnt*dimFrameCnt;
    for (size_t i = 0; i < allSize; ++i)
        frames[i].addPolygons(polygons);
    lastPolyCount = polygons.getSize() - lastPolyCount;

    if (info) {
        ostringstream s;
        s << "\tpolygons to draw: " << lastPolyCount << '\n';
        *info = *info + s.str();
    }
}