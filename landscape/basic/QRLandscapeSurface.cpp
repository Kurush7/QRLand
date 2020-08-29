//
// Created by kurush on 29.07.2020.
//

#include "QRLandscapeSurface.h"

using namespace std;

QRLandscapeSurface::QRLandscapeSurface(const QRMatrix<sptr<QRPoint3D>> &_points)
: points(_points) {
    setTriangulated(true);
    width = points.width();
    height = points.height();

    for (size_t i = 0; i < height-1; ++i) for (size_t j = 0; j < width-1; ++j) {
        // todo
        polygons.push_back(sptr<QRPolygon3D>(new Triangle3D(points[i][j],
                                                            points[i+1][j+1],
                                                            points[i+1][j],
                                                            QRTexturesMap[QRDEFAULT_MATERIAL])));
        polygons.push_back(sptr<QRPolygon3D>(new Triangle3D(points[i][j],
                                                            points[i][j+1],
                                                            points[i+1][j+1],
                                                            QRTexturesMap[QRDEFAULT_MATERIAL])));

            if (!useIndexedPolygons) {
                polygons.push_back(sptr<QRPolygon3D>(new Triangle3D(points[i][j],points[i+1][j+1],
                                                                    points[i+1][j],
                                                                    QRTexturesMap[QRDEFAULT_MATERIAL])));
                polygons.push_back(sptr<QRPolygon3D>(new Triangle3D(points[i][j],points[i][j+1],
                                                                    points[i+1][j+1],
                                                                    QRTexturesMap[QRDEFAULT_MATERIAL])));
            }
            else {
                polygons.push_back(sptr<QRPolygon3D>(new IndexPolygon3D({i*width+j, (i+1)*width+j+1, (i+1)*width+j,},
                                                                    QRTexturesMap[QRDEFAULT_MATERIAL], points.getArray())));
                polygons.push_back(sptr<QRPolygon3D>(new IndexPolygon3D({i*width+j, i*width+j+1, (i+1)*width+j+1,},
                                                                        QRTexturesMap[QRDEFAULT_MATERIAL], points.getArray())));
            }

        /*polygons.push_back(sptr<QRPolygon3D>(
                new Polygon3D({points[i][j],
                               points[i+1][j],
                               points[i+1][j+1],
                               points[i][j+1]},
                               QRTexturesMap[QRDEFAULT_MATERIAL])));*/
    }
}
