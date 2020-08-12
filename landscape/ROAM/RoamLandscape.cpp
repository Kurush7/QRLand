//
// Created by kurush on 11.08.2020.
//

#include "RoamLandscape.h"
#include "RoamTree.cpp"

RoamLandscape::RoamLandscape(const QRMatrix<sptr<QRPoint3D>> &points): points(points) {
    startMeasureTime;
    size_t size = points.rowCnt();
    dimFrameCnt = (size-1) / (FrameSize - 1);
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

void RoamLandscape::updateCamera(const sptr<QRCamera3D> &camera) {
    size_t allSize = dimFrameCnt*dimFrameCnt;
    double d = camera->getScreenDistance();
    RoamUpdateConstant = maxPixelError*maxPixelError /d/d /pixelsPerUnit/pixelsPerUnit;
    auto c = camera->getWorldOriginCoord();
    CameraCoordsX = c[0], CameraCoordsY = c[1], CameraCoordsZ = c[2];

    size_t skipped_polygons = 0;
    for (size_t i = 0; i < allSize; ++i)
        skipped_polygons += frames[i].updateCamera(camera);
}

void RoamLandscape::addPolygons(QRVector<sptr<QRPolygon3D>> &polygons) {
    lastPolyCount = polygons.getSize();
    size_t allSize = dimFrameCnt*dimFrameCnt;
    for (size_t i = 0; i < allSize; ++i)
        frames[i].addPolygons(polygons);
    lastPolyCount = polygons.getSize() - lastPolyCount;
}