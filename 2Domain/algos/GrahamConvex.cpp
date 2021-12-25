//
// Created by kurush on 19.08.2020.
//

#include "GrahamConvex.h"

using namespace std;

double signSquare (Vector3D a, Vector3D b, Vector3D c) {
    return a[0]*(b[1]-c[1])+b[0]*(c[1]-a[1])+c[0]*(a[1]-b[1]);
}

QRVector<Vector3D> makeConvex(const QRVector<Vector3D> &points) {
    int n = points.getSize();
    if (n < 3) return points;

    vector<Vector3D> a;
    for (int i = 0; i < points.getSize(); ++i)
        a.push_back(points[i]);
    sort(a.begin(), a.end());       // todo comparator: check if correct
    Vector3D p1 = a[0],  p2 = a[a.size()-1];
    QRVector<Vector3D> up, down;
    up.push_back (p1);
    down.push_back (p1);
    for (size_t i=1; i<a.size(); ++i) {
        if (i==a.size()-1 || signSquare(p1, a[i], p2) < 0) {
            while (up.getSize()>=2 && signSquare(up[up.getSize()-2], up[up.getSize()-1], a[i]) >= 0)
                up.pop_back();
            up.push_back (a[i]);
        }
        if (i==a.size()-1 || signSquare(p1, a[i], p2) > 0) {
            while (down.getSize()>=2 && signSquare(down[down.getSize()-2], down[down.getSize()-1], a[i]) <= 0)
                down.pop_back();
            down.push_back (a[i]);
        }
    }
    for (size_t i=down.getSize()-2; i>0; --i)
        up.push_back (down[i]);
    return up;
}