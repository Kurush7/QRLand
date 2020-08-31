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
    virtual void process() {
        QRMatrix<float> *local = data.hmap;
        std::map<QRLine2D, double> edges;    // tense, edge
        for (int i = 0; i < data.plates.getSize(); ++i) {
            auto plate = data.plates[i];
            Vector3D move = data.moveVectors[i];
            for (auto &edge: plate->edges) {
                auto normal = len2Norm(edge.getNormal());
                if (normal == XVector || normal == YVector || normal == -1*XVector || normal == -1*YVector) continue;
                double tense = scalar(normal, move);
                if (edges.find(edge) == edges.end())
                    edges[edge] = 0;
                edges[edge] += tense;
            }
        }

        for (auto &x: edges) {
            auto edge = x.first;
            double tense = x.second;
            auto normal = len2Norm(edge.getNormal());
            Vector3D a = edge.a, b = edge.b, eq = len2Norm(edge.getEq());
            a[2] = 1, b[2] = 1;
            float height = tense * plateMountainHeightCoef * data.worldStep;
             float step = data.worldStep;
            auto f = [local, height, a, b, eq, step, normal](size_t x, size_t y){
                Vector3D p(x*step,y*step,1,0);
                float lineDist = fabs(scalar(p, eq));

                float h = height - lineDist*plateMountainSteepCoef * sign(height);
                (*local)[y][x] += h;

                bool outOfEdge;
                p -= lineDist * normal;
                if (fabs(scalar(p, eq)) > QREPS) p += 2 * lineDist * normal;
                if (scalar(lenNorm(p-a), lenNorm(p-b)) < -QREPS)
                    outOfEdge = false;
                else {
                    double dist = min(vectorLen(p-a), vectorLen(p-b));
                    outOfEdge = dist > 2*step;  // todo hardcode
                }

                bool end = height * h < QREPS;
                return (end || outOfEdge);
            };

            BFSWalk(data.width, data.height, a[0]/step, a[1]/step, f);
        }
    }

private:
    std::default_random_engine generator = std::default_random_engine();
};

class PlateMountainsToolCreator: public QRToolCreator {
public:
    virtual uptr<QRTool> create() {return uptr<QRTool>(new PlateMountainsTool());}
};



#endif //BIG3DFLUFFY_PLATEMOUNTAINSTOOL_H
