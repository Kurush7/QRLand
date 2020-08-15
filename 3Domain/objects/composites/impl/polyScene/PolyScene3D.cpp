//
// Created by kurush on 28.06.2020.
//

#include "PolyScene3D.h"

bool PolyScene3D::addModel(const sptr<QRPolyModel3D> &m, const Vector3D &v) {
    auto t = sptr<QRTransformer3D>(MoveTransformer3DCreator(v).create());
    RawModel p = RawModel(m, t);
    models.push_back(p);
}