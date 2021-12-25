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

PolyPosition Polygon3D::where(const Vector3D &v) const {
    auto x = sign(scalar(normal, v) + d);
    //cout << "where: " << v << ' ' << normal << ' ' << d << " => " << (int) x << '\n';
    return (x == 1? FRONT : (x == -1? BEHIND : ON));
}

void Polygon3D::definePlane() {
    if ( points.getSize() < 3)
        throw QRBadParamException(__FILE__, __LINE__, __TIME__,
                                  "failed to set polygon on 2 or less points");

    normal = defineNormal();
    d = -scalar(normal, points[0]->getVector());
    normal[3] = 1;
}

Vector3D Polygon3D::defineNormal() {
    const Vector3D v1 = points[1]->getVector() - points[0]->getVector(),
            v2 = points[2]->getVector() - points[1]->getVector();
    return lenNorm(v1 * v2);
}