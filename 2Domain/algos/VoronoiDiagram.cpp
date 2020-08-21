//
// Created by kurush on 19.08.2020.
//

#include "VoronoiDiagram.h"

// todo <0 >0 change to qreps
// todo 4 frame-points add to check-points
QRVector<sptr<QRFrame2D>> buildVoronoiDiagramOnRect(
        double left, double right, double down, double up,
        const QRVector<Vector3D> &points) {
    QRVector<sptr<QRFrame2D>> polygons;

    QRVector<Vector3D> base_lines;
    base_lines.push_back(QRLine2D(Vector3D(left, down, 0), Vector3D(left, up, 0)).getEq());
    base_lines.push_back(QRLine2D(Vector3D(left, down, 0), Vector3D(right, down, 0)).getEq());
    base_lines.push_back(QRLine2D(Vector3D(left, up, 0), Vector3D(right, up, 0)).getEq());
    base_lines.push_back(QRLine2D(Vector3D(right, down, 0), Vector3D(right, up, 0)).getEq());
    QRLine2D ln;

    QRVector<Vector3D> lines;
    //for (int i = 0; i < 1; ++i) {
    for (int i = 0; i < points.getSize(); ++i) {
        // get median perpendicular bisectors. side with p_i is > 0
        lines.clear();
        for (auto &x: base_lines)
            lines.push_back(x);

        Vector3D p = points[i];
        p[3] = 0, p[2] = 1;

        for (int j = 0; j < points.getSize(); ++j) {
            if (i == j) continue;
            ln = QRLine2D(p, points[j]);
            Vector3D vec = ln.getEq(); vec[2] = 0; vec[3] = 0;
            Vector3D midP = (p + points[j]) / 2.;
            double tmp = vec[0];
            vec[0] = -vec[1];
            vec[1] = tmp;

            vec[2] = -vec[0]*midP[0] - vec[1]*midP[1];
            vec[3] = 0;
            lines.push_back(vec);
        }
        for (int j = 0; j < lines.getSize(); ++j)
            if (scalar(p, lines[j]) < 0) lines[j] = -1*lines[j];


        // get intersection points
        QRVector<Vector3D> points;
        char status;
        Vector3D pt;
        for (int j = 0; j < lines.getSize(); ++j)
            for (int k = j+1; k < lines.getSize(); ++k) {
                pt = intersectLines(lines[j], lines[k], &status);
                pt[2] = 1;
                if (status == 1)
                    points.push_back(pt);
            }

        QRVector<Vector3D> goodPoints;
        for (int j = 0; j < points.getSize(); ++j) {
            bool flag = 1;
            for (int k = 0; k < lines.getSize(); ++k) {
                if (scalar(points[j], lines[k]) < -QREPS) {
                    flag = 0;
                    break;
                }
            }
            if (flag)
                goodPoints.push_back(points[j]);
        }

        goodPoints = makeConvex(goodPoints);    // graham

        polygons.push_back(sptr<QRFrame2D>(new QRFrame2D(goodPoints)));
    }
    return polygons;
}