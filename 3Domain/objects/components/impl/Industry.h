//
// Created by kurush on 27.06.2020.
//

#ifndef BIG3DFLUFFY_INDUSTRY_H
#define BIG3DFLUFFY_INDUSTRY_H

#include "../interfaces/QRIndustry.h"
#include "Point3D.h"
#include "Polygon3D.h"

class Point3DCreator: public QRPoint3DCreator {
public:
    explicit Point3DCreator(QRPointStyle s = QRPointStyle()): st(s) {}
    virtual uptr<QRPoint3D> create(const Vector3D &vec) {
        return uptr<QRPoint3D>(new Point3D(vec, st));
    }
private:
    QRPointStyle st;
};

class Polygon3DCreator: public QRPolygon3DCreator {
public:
    Polygon3DCreator(const sptr<QRTexture> txt = DEFAULT_TEXTURE): texture(txt) {}
    virtual uptr<QRPolygon3D> create(const QRVector<sptr<QRPoint3D>> &p) {
            return uptr<QRPolygon3D>(new Polygon3D(p, texture));
    }
    virtual uptr<QRPolygon3D> create(initializer_list<sptr<QRPoint3D>> lst) {
        return uptr<QRPolygon3D>(new Polygon3D(lst, texture));
    }

private:
    sptr<QRTexture> texture;
};


#endif //BIG3DFLUFFY_INDUSTRY_H

