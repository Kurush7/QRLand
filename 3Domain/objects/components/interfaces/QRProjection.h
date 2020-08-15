//
// Created by kurush on 30.05.2020.
//

#ifndef BIG3DFLUFFY_QRPROJECTION_H
#define BIG3DFLUFFY_QRPROJECTION_H

#include "math/QRMath.h"

class ProjectionImp {
public:
    virtual uptr<QRTransformer3D> getTransformer(const Vector3D &origin,
                                                 const Vector3D &zeroPoint, const Vector3D &ViewUpVector) = 0;
};


#endif //BIG3DFLUFFY_QRPROJECTION_H
