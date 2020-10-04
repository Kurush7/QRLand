//
// Created by kurush on 20.08.2020.
//

#include "BFSWalk.h"
#include "../random_generator.h"

using namespace std;

bool is_valid(int w, int h, int x, int y) {
    return (x>=0 && y>=0 && x<w && y < h);
}

void BFSWalk(QRQueue<QRPair<int, int>> &start_q, int w, int h, QRMatrix<float> *hmap) {
    queue<pair<int, int>> stack;
    map<pair<int, int>, bool> used;

    int dx[] = {0,0,1,-1}, dy[] = {1,-1,0,0};
    int start_cnt = 0;
    while (! start_q.isEmpty()) {
        start_cnt++;
        auto p = start_q.pop();
        used[{p.fst, p.snd}] = true;
        stack.push({p.fst, p.snd});
    }
    while (!stack.empty()) {
        int x = stack.front().first, y = stack.front().second;
        stack.pop();

        if (start_cnt <= 0) {
            float tmp = 0;
            int cnt = 0;
            if (x > 0) tmp += (*hmap)[y][x - 1], cnt++;
            if (x < w - 1) tmp += (*hmap)[y][x + 1], cnt++;
            if (y > 0) tmp += (*hmap)[y - 1][x], cnt++;
            if (y < h - 1) tmp += (*hmap)[y + 1][x], cnt++;
            tmp /= cnt;
            // todo add random
            if (fabs((*hmap)[y][x] - tmp) < QREPS_MINI) continue; //todo
            (*hmap)[y][x] = tmp;
        }
        start_cnt--;

        for (int i = 0; i < 4; ++i) {
            int x1 = x+dx[i], y1 = y+dy[i];
            if (!is_valid(w, h, x1, y1)) continue;
            if (used.find({x1, y1}) != used.end()) continue;
            used[{x1,y1}] = true;
            stack.push({x1,y1});
        }
    }
}