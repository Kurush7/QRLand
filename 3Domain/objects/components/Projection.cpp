//
// Created by kurush on 30.05.2020.
//

#include "Projection.h"

using namespace std;

unique_ptr<BaseTransformer3D> OrthogonalProjection::getTransformer(const Vector3D &origin,
                                         const Vector3D &zeroPoint, const Vector3D &ViewUpVector) {
    auto oz = lenNorm(zeroPoint - origin);
    auto oy = lenNorm(ViewUpVector);
    ProjectionTransformer3DCreator creator(zeroPoint, oz * oy, oy, oz);
    return creator.create();
    //todo normalize, minus ox??!!!!
}