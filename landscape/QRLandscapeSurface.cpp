//
// Created by kurush on 29.07.2020.
//

#include "QRLandscapeSurface.h"

using namespace std;

QRLandscapeSurface::QRLandscapeSurface(size_t w, size_t h, double step): points(w ,h), step(step) {
    width = w;
    height = h;
    for(size_t i = 0; i < h; ++i)
        for(size_t j = 0; j < w; ++j)
            points[i][j] = sptr<QRPoint3D>(new Point3D((j-(double)(w)/2)*step,(i-(double)(h)/2)*step,0));

    for (size_t i = 0; i < h-1; ++i)
        for (size_t j = 0; j < w-1; ++j) {
            polygons.push_back(sptr<QRPolygon3D>(new Triangle3D(points[i][j],
                                                                points[i+1][j+1],
                                                                points[i+1][j],
                                                                QRTexturesMap[QRDEFAULT_MATERIAL])));
            polygons.push_back(sptr<QRPolygon3D>(new Triangle3D(points[i][j],
                                                                points[i][j+1],
                                                                points[i+1][j+1],
                                                                QRTexturesMap[QRDEFAULT_MATERIAL])));
            /*QRVector<sptr<QRPoint3D>> pts(3);
            pts[0] = points[i][j];
            pts[1] = points[i+1][j+1];
            pts[2] = points[i+1][j];
            auto p = sptr<QRPolygon3D>(new Polygon3D(pts, QRTexturesMap[QRDEFAULT_MATERIAL]));
            polygons.push_back(p);

            pts[1] = points[i][j+1];    // another rotate order. to keep normals on the same side
            pts[2] = points[i+1][j+1];
            p = sptr<QRPolygon3D>(new Polygon3D(pts, QRTexturesMap[QRDEFAULT_MATERIAL]));
            polygons.push_back(p);*/
        }
}

QRLandscapeSurface::QRLandscapeSurface(initializer_list<initializer_list<double>> initList, double step)
: QRLandscapeSurface(initList.begin()->size(), initList.size(), step) {
    size_t i=0, j;
    for (auto &row: initList) {
        j = 0;
        for (double elem: row) {
            auto vec = points[i][j]->getVector();
            vec[2] = elem;
            points[i][j]->setVector(vec);
            j++;
        }
        i++;
    }
}

QRLandscapeSurface::QRLandscapeSurface(const QRMatrix<double> &m, double step)
:QRLandscapeSurface(m.width(), m.height(), step) {
    for(size_t i = 0; i < height; ++i)
        for(size_t j = 0; j < width; ++j) {
            auto vec = points[i][j]->getVector();
            vec[2] = m[i][j];
            points[i][j]->setVector(vec);
        }
}