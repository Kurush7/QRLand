//
// Created by kurush on 02.08.2020.
//

#include "Triangle3D.h"

Triangle3D::Triangle3D(sptr<QRPoint3D> p1, sptr<QRPoint3D> p2, sptr<QRPoint3D> p3,
        const sptr<QRTexture> t)
: p1(p1), p2(p2), p3(p3), texture(t) {
    arr[0] = p1, arr[1] = p2, arr[2] = p3;
    definePlane();
}
Triangle3D::Triangle3D(initializer_list<sptr<QRPoint3D>> lst, const sptr<QRTexture> t ): texture(t) {
    int i = 0;
    for (auto &elem: lst) {
        if (i == 0) p1 = elem;
        else if (i == 1) p2 = elem;
        else p3 = elem;
        i++;
        if (i == 3) break;
    }
    arr[0] = p1, arr[1] = p2, arr[2] = p3;
    definePlane();
}

bool Triangle3D::operator==(const QRPolygon3D &b) const {
    // todo check cycle-shift
    int i = 0;
    auto it = b.getPoints();
    for (; it; ++it) {
        if (i == 0) { if (p1.get() != it->get()) return false; }
        else if (i == 1) { if (p2.get() != it->get()) return false; }
        else { if (p3.get() != it->get()) return false; }
        i++;
        if (i == 3) break;
    }
    arr[0] = p1, arr[1] = p2, arr[2] = p3;
    return !(bool)it;     // if it=True, then it's not ended -> more than 3 nodes
}

PolyPosition Triangle3D::where(const Vector3D &v) const {
    auto x = sign(scalar(normal, v) + d);
    return (x == 1? FRONT : (x == -1? BEHIND : ON));
}

void Triangle3D::definePlane() {
    normal = defineNormal();
    d = -scalar(normal, p1->getVector());
    normal[3] = 1;
}

Vector3D Triangle3D::defineNormal() {
    const Vector3D v1 = p2->getVector() - p1->getVector(),
            v2 = p3->getVector() - p2->getVector();
    return lenNorm(v1 * v2);
}