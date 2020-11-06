//
// Created by kurush on 20.08.2020.
//

#include "BFSWalk.h"
#include "../../random_generator.h"

using namespace std;

bool is_valid(int w, int h, int x, int y) {
    return (x>=0 && y>=0 && x<w && y < h);
}

void BFSMountainWalk(QRQueue<QRPair<int, int>> &start_q, int w, int h, QRMatrix<float> *hmap, float worldStep) {
    queue<pair<int, int>> queue, queue1;
    map<pair<int, int>, bool> used;

    uniform_real_distribution<float> shift(-worldStep*mountainsDSRandomCoef,
                                           worldStep*mountainsDSRandomCoef);
    // todo separate randoms
    int dx[] = {0,0,1,-1}, dy[] = {1,-1,0,0};
    // todo shuffle it

    int start_cnt = 0;
    while (! start_q.isEmpty()) {
        start_cnt++;
        auto p = start_q.pop();
        used[{p.fst, p.snd}] = true;
        queue.push({p.fst, p.snd});
        queue1.push({p.fst, p.snd});
    }
    while (!queue.empty() || !queue1.empty()) {
        int x, y;
        if (!queue.empty()) {
            x = queue.front().first, y = queue.front().second;
            queue.pop();
        }
        else {
            x = queue1.front().first, y = queue1.front().second;
            queue1.pop();
        }

        if (start_cnt <= 0) {
            float tmp = 0;
            float k = 1;
            float cnt = 0;
            if (x > 0) {
                k = used.find({x-1, y}) == used.end()? 1 : mountainPriorCoef;
                tmp += (*hmap)[y][x - 1]*k, cnt+=k;
            }
            if (x < w-1) {
                k = used.find({x+1, y}) == used.end()? 1 : mountainPriorCoef;
                tmp += (*hmap)[y][x + 1]*k, cnt+=k;
            }
            if (y > 0) {
                k = used.find({x, y-1}) == used.end()? 1 : mountainPriorCoef;
                tmp += (*hmap)[y-1][x]*k, cnt+=k;
            }
            if (y < h-1) {
                k = used.find({x, y+1}) == used.end()? 1 : mountainPriorCoef;
                tmp += (*hmap)[y+1][x]*k, cnt+=k;
            }

            tmp /= cnt;
            tmp += shift(default_generator);
            if (fabs((*hmap)[y][x] - tmp) < worldStep/mountainStopCoef) continue;
            (*hmap)[y][x] = tmp;
        }
        start_cnt--;

        for (int i = 0; i < 4; ++i) {
            int x1 = x+dx[i], y1 = y+dy[i];
            if (!is_valid(w, h, x1, y1)) continue;
            if (used.find({x1, y1}) != used.end()) continue;
            used[{x1,y1}] = true;
            queue.push({x1, y1});
        }
    }
}



void BFSHillWalk(QRQueue<QRPair<int, int>> &start_q, int w, int h, QRMatrix<float> *hmap, float worldStep) {
    queue<pair<int, int>> queue, queue1;
    map<pair<int, int>, bool> used;

    uniform_real_distribution<float> shift(-worldStep*diamondSquareRandomUpdateCoef,
            worldStep*diamondSquareRandomUpdateCoef);
    int dx[] = {0,0,1,-1}, dy[] = {1,-1,0,0};
    // todo shuffle it

    int start_cnt = 0;
    while (! start_q.isEmpty()) {
        start_cnt++;
        auto p = start_q.pop();
        used[{p.fst, p.snd}] = true;
        queue.push({p.fst, p.snd});
        queue1.push({p.fst, p.snd});
    }
    while (!queue.empty() || !queue1.empty()) {
        int x, y;
        if (!queue.empty()) {
            x = queue.front().first, y = queue.front().second;
            queue.pop();
        }
        else {
            x = queue1.front().first, y = queue1.front().second;
            queue1.pop();
        }

        if (start_cnt <= 0) {
            float tmp = 0;
            int cnt = 0;
            if (x > 0) tmp += (*hmap)[y][x - 1], cnt++;
            if (x < w - 1) tmp += (*hmap)[y][x + 1], cnt++;
            if (y > 0) tmp += (*hmap)[y - 1][x], cnt++;
            if (y < h - 1) tmp += (*hmap)[y + 1][x], cnt++;
            tmp /= cnt;
            tmp += shift(default_generator);
            if (fabs((*hmap)[y][x] - tmp) < worldStep/hillStopCoef) continue; //todo
            (*hmap)[y][x] = tmp;
        }
        start_cnt--;

        for (int i = 0; i < 4; ++i) {
            int x1 = x+dx[i], y1 = y+dy[i];
            if (!is_valid(w, h, x1, y1)) continue;
            if (used.find({x1, y1}) != used.end()) continue;
            used[{x1,y1}] = true;
            queue.push({x1, y1});
        }
    }
}