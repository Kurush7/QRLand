//
// Created by kurush on 19.08.2020.
//

#ifndef BIG3DFLUFFY_SCALETRANSFORMER_H
#define BIG3DFLUFFY_SCALETRANSFORMER_H

#include "QRTransformer2D.h"

class ScaleTransformer2D: public QRTransformer2D {
public:
    ScaleTransformer2D(double kx, double ky): kx(kx), ky(ky) {}
    virtual Vector3D transform(const Vector3D &vec) {
        return Vector3D(vec[0]*kx, vec[1]*ky, 0, 0);
    }

private:
    double kx, ky;
};


#endif //BIG3DFLUFFY_SCALETRANSFORMER_H
