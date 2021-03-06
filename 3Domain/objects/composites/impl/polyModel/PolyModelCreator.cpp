//
// Created by kurush on 27.06.2020.
//

#include "PolyModelCreator.h"

bool CubeModelCreator::createPoints() {
    float b = a / 2;
    for (float x = -b; x < +3*b; x += a)
        for (float y = -b; y < 3*b; y += a)
            for (float z = -b; z < +3*b; z += a)
                points.push_back(sptr<QRPoint3D>(new Point3D(x,y,z)));
    return true;
}
bool CubeModelCreator::createPolygons() {
    int arr[6][4] = {{0,1,3,2}, {4,5,7,6}, {0,1,5,4}, {1,3,7,5}, {2,3,7,6}, {0,2,6,4}};
    for (int i = 0; i < 6; ++i)
        if (useIndexedPolygons)
            polygons.push_back(sptr<QRPolygon3D>(
                    new IndexPolygon3D({arr[i][0], arr[i][1],
                                   arr[i][2], arr[i][3]}, texture, points)));
        else
            polygons.push_back(sptr<QRPolygon3D>(
                    new Polygon3D({points[arr[i][0]], points[arr[i][1]],
                                       points[arr[i][2]], points[arr[i][3]]}, texture)));

    for (auto p: polygons)
        if (p->where(ZeroVector) == FRONT)
            p->switchNormal();

    return true;
}

sptr<QRPolyModel3D> CubeModelCreator::getModel() {
    model = sptr<QRPolyModel3D>(new PolyModel(points.begin(), polygons.begin()));
    //model->setConvexity(true);
    return model;
}