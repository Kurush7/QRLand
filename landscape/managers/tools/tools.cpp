//
// Created by kurush on 04.10.2020.
//

#include "HillTool.h"
#include "PlateMountainsTool.h"
#include "../../random_generator.h"

void PlateMountainsTool::setToolData(const ToolData &dt) {
    data = dt;
    give = data.worldStep*2;
    if (!inited) {
        height = 1e9; // todo!!!
        inited=true;
    }

    edges.clear();
    for (int i = 0; i < data.plates.getSize(); ++i) {
        auto plate = data.plates[i];
        Vector3D move = data.moveVectors[i];
        for (auto &edge: plate->edges) {
            auto normal = len2Norm(edge.getNormal());
            if (normal == XVector || normal == YVector || normal == -1*XVector || normal == -1*YVector) {
                auto w = data.width * data.worldStep, h = data.width * data.worldStep;
                if (edge.a[0] == 0 && edge.b[0] == 0) continue;
                if (edge.a[0] == w && edge.b[0] == w) continue;
                if (edge.a[1] == 0 && edge.b[1] == 0) continue;
                if (edge.a[1] == h && edge.b[1] == h) continue;
            }
            Vector3D tense = move;      // moves len = 1
            if (edges.find(edge) == edges.end()) {
                edges[edge] = {{plate->getCenter()}, {tense}};
            }
            else edges[edge].snd.push_back(tense), edges[edge].fst.push_back(plate->getCenter());
        }
    }
}

void HillTool::setToolData(const ToolData &dt) {
    data = dt;
    if (!inited) {
        inited=true;
        float maxH = maxHillHeightParam * data.worldStep;
        uniform_real_distribution<float> dist_hill(minHillHeightParam * data.worldStep, maxH);
        uniform_int_distribution<size_t> dist_w(0, data.width - 1);
        uniform_int_distribution<size_t> dist_h(0, data.height - 1);

        centerX = dist_w(default_generator);
        centerY = dist_h(default_generator);
        height = dist_hill(default_generator);
        give = data.worldStep*2;
    }
    else {
        // todo
        centerX *= 2;
        centerY *= 2;
        give = data.worldStep*2;
    }
}

void HillSetTool::setToolData(const ToolData &dt) {
    data = dt;
    if (!inited) {
        inited = true;
    }
    for (auto &t: tools)
        t.setToolData(data);
}