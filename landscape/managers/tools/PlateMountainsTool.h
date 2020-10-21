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

    virtual bool process() {
        QRMatrix<float> *local = data.hmap;

        float max_w = data.width * data.worldStep;
        float max_h = data.height * data.worldStep;
        for (auto &e: edges) {
            QRQueue<QRPair<int, int>> q;

            auto edge = e.first;
            double tense = minPlateMoveForce + e.second * maxPlateMoveForce;
            if (fabs(tense) < mountainsMinTense)
                tense = sign(tense) * mountainsMinTense;
            float dh = tense * plateMountainHeightCoef * data.worldStep;

            Vector3D a = edge.a, b = edge.b;
            a /= data.worldStep, b /= data.worldStep;
            float dx = b[0] - a[0], dy = b[1]-a[1];
            if (dx > dy) {
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
                    q.push({xi, yi});
                    (*data.hmap)[yi][xi] += dh;
                }
            }

            BFSMountainWalk(q, data.width, data.height, data.hmap, data.worldStep);
        }
        return true;
    }

private:
    std::map<QRLine2D, double> edges;
    bool inited=false;
    float height, give;
};

class PlateMountainsToolCreator: public QRToolCreator {
public:
    virtual uptr<QRTool> create() {return uptr<QRTool>(new PlateMountainsTool());}
};



#endif //BIG3DFLUFFY_PLATEMOUNTAINSTOOL_H
