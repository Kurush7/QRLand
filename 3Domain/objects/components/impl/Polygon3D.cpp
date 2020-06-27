//
// Created by kurush on 26.06.2020.
//

#include "Polygon3D.h"

Polygon3D::Polygon3D(const QRVector<sptr<QRPoint3D>> &p, const sptr<QRTexture> t): points(p), texture(t) {
    definePlane();
}
Polygon3D::Polygon3D(initializer_list<sptr<QRPoint3D>> lst, const sptr<QRTexture> t ): points(lst), texture(t) {
    definePlane();
}

bool Polygon3D::operator==(const QRPolygon3D &b) const {
    // todo check cycle-shift
    auto my = points.begin(), it = b.getPoints();
    for (; it && my != points.end(); ++it, ++my)
        if (*my != *it)
            return false;
    return !(my || it);     // have same dimension
}

PolyPosition Polygon3D::where(const Vector3D &v) const {
    auto x = sign(v[0]*normal[0] + v[1]*normal[1] + v[2]*normal[2] + v[3]*d);
    return (x == 1? FRONT : (x == -1? BEHIND : ON));
}

void Polygon3D::definePlane() {
    if ( points.getSize() < 3)
        throw QRBadSizeException(__FILE__, __LINE__, __TIME__,
                "failed to init polygon with 2 or less points");

    const Vector3D v1 = points[1]->getPoint() - points[0]->getPoint(),
                   v2 = points[2]->getPoint() - points[1]->getPoint();
    normal = lenNorm(v1 * v2);
    d = normal[3];
    normal[3] = 1;
}