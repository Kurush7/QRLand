//
// Created by kurush on 16.08.2020.
//

#ifndef BIG3DFLUFFY_PLATEMOUNTAINSTOOL_H
#define BIG3DFLUFFY_PLATEMOUNTAINSTOOL_H

#include "QRTool.h"
#include "BFSWalk.h"

#include <map>


class PlateMountainsTool: public QRTool {
public:


    virtual void setToolData(const ToolData &dt);
    virtual void setIntensity(float x) {
        intensity = x;}

    virtual bool process() {
        QRMatrix<float> *local = data.hmap;

        float max_w = data.width * data.worldStep;
        float max_h = data.height * data.worldStep;
        for (auto &e: edges) {
            QRQueue<QRPair<int, int>> q;
            auto edge = e.first;
            double tense, dh, tvec;

            Vector3D a = edge.a, b = edge.b;
            a /= data.worldStep, b /= data.worldStep;
            float dx = b[0] - a[0], dy = b[1]-a[1];
            if (fabs(dx) > fabs(dy)) {
                dy /= fabs(dx);
                dx /= fabs(dx);
            }
            else {
                dx /= fabs(dy);
                dy /= fabs(dy);
            }
            int step_cnt = (b[0]-a[0]) / dx;
            float x = a[0], y = a[1];
            int xi, yi;
            for (int i = 0; i < step_cnt; ++i) {
                xi = QRound(x), yi = QRound(y);
                x += dx, y += dy;
                if (xi >= 0 and xi < data.width and yi >= 0 and yi < data.height) {
                    tense = 0;
                    for (int f = 0; f < e.second.snd.getSize(); ++f) {
                        tvec = cos2(e.second.snd[f], len2Norm(Vector3D(x, y, 0) - e.second.fst[f]));
                        tvec = sign(tvec) * tvec * tvec;
                        tense += tvec;
                    }
                    tense = sign(tense) * minPlateMoveForce + tense * (maxPlateMoveForce - minPlateMoveForce);
                    if (fabs(tense) < mountainsMinTense)
                        tense = sign(tense) * mountainsMinTense;
                    dh = tense * plateMountainHeightCoef * data.worldStep * intensity;

                    q.push({xi, yi});
                    (*data.hmap)[yi][xi] += dh;
                }
            }
            cout << '\n';

            BFSMountainWalk(q, data.width, data.height, data.hmap, data.worldStep);
        }
        cout << "\n============\n\n";
        return true;
    }

private:
    std::map<QRLine2D, QRPair<QRVector<Vector3D>, QRVector<Vector3D>>> edges;   // center, move
    bool inited=false;
    float height, give;
    float intensity=1;
};

class PlateMountainsToolCreator: public QRToolCreator {
public:
    virtual uptr<QRTool> create() {return uptr<QRTool>(new PlateMountainsTool());}
};



#endif //BIG3DFLUFFY_PLATEMOUNTAINSTOOL_H
