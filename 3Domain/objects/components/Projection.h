//
// Created by kurush on 30.05.2020.
//

#ifndef BIG3DFLUFFY_PROJECTION_H
#define BIG3DFLUFFY_PROJECTION_H

#include "../../math/QRMath.h"

class ProjectionImp {
public:
    virtual std::unique_ptr<BaseTransformer3D> getTransformer(const Vector3D &origin,
            const Vector3D &zeroPoint, const Vector3D &ViewUpVector) = 0;
};

class OrthogonalProjection: public ProjectionImp {
public:
    virtual std::unique_ptr<BaseTransformer3D> getTransformer(const Vector3D &origin,
                                             const Vector3D &zeroPoint, const Vector3D &ViewUpVector);
};


#endif //BIG3DFLUFFY_PROJECTION_H
