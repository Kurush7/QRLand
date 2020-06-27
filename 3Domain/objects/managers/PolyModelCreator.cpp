//
// Created by kurush on 27.06.2020.
//

#include "PolyModelCreator.h"

bool CubeModelCreator::createPoints() {
    double b = a / 2;
    for (double x = pos[0] - b; x < pos[0]+3*b; x += a)
        for (double y = pos[1] - b; y < pos[1]+3*b; y += a)
            for (double z = pos[2] - b; z < pos[2]+3*b; z += a)
                points.push_back(sptr<QRPoint3D>(new Point3D(x,y,z)));
}
bool CubeModelCreator::createPolygons() {
    int arr[6][4] = {{0,1,2,3}, {4,5,6,7}, {0,1,4,5}, {1,3,5,7}, {2,3,6,7}, {0,2,4,6}};
    for (int i = 0; i < 6; ++i)
        polygons.push_back(sptr<QRPolygon3D>(
                new Polygon3D({points[arr[i][0]], points[arr[i][1]],
                                   points[arr[i][2]], points[arr[i][3]]}, texture)));

    for (auto p: polygons)
        if (p->where(pos) == FRONT)
            p->switchNormal();
}

sptr<QRPolyModel3D> CubeModelCreator::getModel() {
    model = sptr<QRPolyModel3D>(new PolyModel(points.begin(), polygons.begin()));
    return model;
}