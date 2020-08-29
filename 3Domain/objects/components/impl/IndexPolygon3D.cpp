//
// Created by kurush on 29.08.2020.
//

#include "IndexPolygon3D.h"

IndexPolygon3D::IndexPolygon3D(const QRVector<int32_t> &ind,
               const sptr<QRTexture> t,
               const QRVector<sptr<QRPoint3D>> &pts): points(ind), texture(t) {
    definePlane(pts);
}

IndexPolygon3D::IndexPolygon3D(const QRVector<int32_t> &ind,
               const sptr<QRTexture> t,
               const Vector3D &plane): points(ind), texture(t) {
    normal = plane;
    d = plane[3];
    normal[3] = 1;
}

IndexPolygon3D::IndexPolygon3D(initializer_list<int32_t> lst,
        const sptr<QRTexture> t,
        const QRVector<sptr<QRPoint3D>> &pts): points(lst), texture(t) {
    definePlane(pts);
}

PolyPosition IndexPolygon3D::where(const Vector3D &v) const {
    auto x = sign(scalar(normal, v) + d);
    return (x == 1? FRONT : (x == -1? BEHIND : ON));
}

void IndexPolygon3D::definePlane(const QRVector<sptr<QRPoint3D>> &pts) {
    if ( points.getSize() < 3)
        throw QRBadParamException(__FILE__, __LINE__, __TIME__,
                                  "failed to set polygon on 2 or less points");

    const Vector3D v1 = pts[points[0]]->getVector() - pts[points[1]]->getVector(),
            v2 = pts[points[2]]->getVector() - pts[points[1]]->getVector();
    normal = lenNorm(v1 * v2);
    d = -scalar(normal, pts[points[0]]->getVector());
    normal[3] = 1;
}