//
// Created by kurush on 20.08.2020.
//

#include "BFSWalk.h"

using namespace std;

bool is_valid(size_t w, size_t h, size_t x, size_t y) {
    return (x>=0 && y>=0 && x<w && y < h);
}

void BFSWalk(size_t w, size_t h, size_t startX, size_t startY,
             std::function<bool(size_t, size_t)> func) {
    if (!is_valid(w,h,startX,startY)) return;
    queue<pair<size_t, size_t>> stack;
    map<pair<size_t, size_t>, bool> used;

    int dx[] = {0,0,1,-1}, dy[] = {1,-1,0,0};

    stack.push({startX,startY});
    used[{startX,startY}] = true;
    while (!stack.empty()) {
        size_t x = stack.front().first, y = stack.front().second;
        stack.pop();
        bool end = func(x,y);
        if (end) continue;
        for (int i = 0; i < 4; ++i) {
            size_t x1 = x+dx[i], y1 = y+dy[i];
            if (!is_valid(w, h, x1, y1)) continue;
            if (used.find({x1, y1}) != used.end()) continue;
            used[{x1,y1}] = true;
            stack.push({x1,y1});
        }
    }
}