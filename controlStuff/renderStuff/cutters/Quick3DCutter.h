//
// Created by kurush on 29.08.2020.
//

#ifndef BIG3DFLUFFY_QUICK3DCUTTER_H
#define BIG3DFLUFFY_QUICK3DCUTTER_H

#include "QRCutter.h"
#include "../render/QuickRenderData.h"

class Quick3DCutter {
public:
    Quick3DCutter(QuickRenderData &data): data(data) {}
    void cutPoly(size_t ind);
    void setCutter(const Vector3D *planes, int cnt);
    ~Quick3DCutter() {
        delete[] interP;
        if (cutterSize) {
            for (int i = 0; i < cutterSize; ++i)
                delete[] cutter[i];
            delete[] cutter;
        }

    }
    // todo inside values must be > 0!!!!
private:
    QuickRenderData& data;

    void innerCutter(size_t ind);

    inline char getCode(float *v);
    inline void intersectionPoint(float *p1, float *p2, int cut_i);

    int Nw, Np;
    QRVector<int32_t> P, Q;
    float **cutter = nullptr;
    int cutterSize = 0; // todo this = Nw
    float *interP = new float[3];
    int32_t S;
    bool interFlag;
};

#endif //BIG3DFLUFFY_QUICK3DCUTTER_H
