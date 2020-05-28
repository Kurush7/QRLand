//
// Created by kurush on 11.04.2020.
//

#include "DropPolyFiller.h"
#include <iostream>

using namespace std;


void DropPolyFiller::draw(QPainter &p, Transformer *t, bool setStyle) {
    if (debugging)
        return;
    p.setPen(lineColor);
    for (auto line: lines)
        line->draw(p, t, false);
    intDrop = IntPoint(t->fit(drop));
    if (fill) {
        vector<Edge> edges;
        fillFigure(edges);
    }
    fill = false;
}

inline void DropPolyFiller::updateStackLine(int left, int right, int y, stack<IntPoint> &st) {
    int x = left;
    bool flag;
    QColor color;
    while (x <= right) {
        color = image->pixelColor(x,y);
        flag = color != lineColor && color != fillColor && x <= right;
        while(color != lineColor && color != fillColor && x <= right) {
            x++;
            if (x <= right) color = image->pixelColor(x,y);
        }
        if (flag) {
            if (x == right && color != fillColor && color != lineColor)
                st.push(IntPoint(x,y));
            else
                st.push(IntPoint(x-1,y));
        }
        int x0 = x;
        while((color == lineColor || color == fillColor) && x <= right) {
            x++;
            if (x <= right) color = image->pixelColor(x,y);
        }
        if (x == x0)
            x++;
    }
}

int DropPolyFiller::fillFigure(vector<Edge> &edges) {
    stack<IntPoint> st;
    st.push(intDrop);

    IntPoint curDrop;
    int x, y;
    int left, right;
    int width = image->width(), height = image->height();
    while (!st.empty()) {
        curDrop = st.top();
        st.pop();

        x = curDrop.x, y = curDrop.y;
        while (x < width && image->pixelColor(x,y) != lineColor) {
            image->setPixelColor(x, y, fillColor);
            x++;
        }
        right = x - (ifCoh4 || x == width);
        x = curDrop.x;
        while (x >= 0 && image->pixelColor(x,y) != lineColor) {
            image->setPixelColor(x, y, fillColor);
            x--;
        }
        left = x + (ifCoh4 || x < 0);

        if (y < height - 1) updateStackLine(left, right, y+1, st);
        if (y > 0) updateStackLine(left, right, y-1, st);
    }
}


void DropPolyFiller::initDebug(Transformer *t) {
    debugging = true;
    while (!debug.st.empty())
        debug.st.pop();
    debug.edges.clear();
    for (auto line: lines) {
        auto pa = t->fit(line->a);
        auto pb = t->fit(line->b);
        debug.edges.push_back(Edge(pa, pb));
    }
    debug.cur_line = 0;
    debug.i = 0;
    debug.movingLeft = true;
    debug.x = intDrop.x;
    debug.y = intDrop.y;
    debug.x0 = debug.x;
    image->setPixelColor(debug.x, debug.y, fillColor);
}

void DropPolyFiller::debugLine() {
    int line = debug.cur_line;
    while (debug.cur_line == line && debugging)
        debugNext();
}

void DropPolyFiller::debugNext() {
    if (debug.movingLeft) {
        debug.x--;
        if (debug.x == -1) {
            debugging = false;
            return;
        }
        if(image->pixelColor(debug.x, debug.y) != lineColor) {
            image->setPixelColor(debug.x, debug.y, fillColor);
        }
        else {
            debug.left = debug.x + ifCoh4;
            debug.x = debug.x0;
            debug.movingLeft = false;
        }
    }
    else {
        debug.x++;
        if (debug.x == image->width()) {
            debugging = false;
            return;
        }
        if(image->pixelColor(debug.x, debug.y) != lineColor) {
            image->setPixelColor(debug.x, debug.y, fillColor);
        }
        else {
            debug.right = debug.x - ifCoh4;
            updateStackLine(debug.left, debug.right, debug.y+1, debug.st);
            updateStackLine(debug.left, debug.right, debug.y-1, debug.st);
            if (debug.st.empty()) {
                debugging = false;
                return;
            }
            debug.x = debug.st.top().x;
            debug.x0 = debug.x;
            debug.y = debug.st.top().y;
            debug.st.pop();
            debug.cur_line++;
            debug.movingLeft = true;
            image->setPixelColor(debug.x, debug.y, fillColor);
        }
    }
}


inline void DropPolyFiller::updateStackLineStats(int left, int right, int y, stack<IntPoint> &st) {
    int x = left;
    bool flag;
    QColor color;
    while (x <= right) {
        flag = false;
        color = image->pixelColor(x,y);
        statRead++;
        while(color != lineColor && color != fillColor && x <= right) {
            flag = true;
            x++;
            if (x <= right) color = image->pixelColor(x,y), statRead++;
        }
        if (flag) {
            if (x == right && color != fillColor && color != lineColor)
                st.push(IntPoint(x,y));
            else
                st.push(IntPoint(x-1,y));
        }
        int x0 = x;
        while((color == lineColor || color == fillColor) && x <= right) {
            x++;
            if (x <= right) color = image->pixelColor(x,y), statRead++;
        }
        if (x == x0)
            x++;
    }
}

int DropPolyFiller::fillFigureStats(vector<Edge> edges) {
    stack<IntPoint> st;
    st.push(intDrop);

    IntPoint curDrop;
    int x, y;
    int left, right;
    int width = image->width(), height = image->height();
    while (!st.empty()) {
        curDrop = st.top();
        st.pop();

        x = curDrop.x, y = curDrop.y;
        while (x < width && image->pixelColor(x,y) != lineColor) {
            image->setPixelColor(x, y, fillColor);
            statRead++;
            statWrite++;
            x++;
        }
        if (x < width) statRead++;
        right = x - (ifCoh4 || x == width);
        x = curDrop.x - 1;
        while (x >= 0 && image->pixelColor(x,y) != lineColor) {
            image->setPixelColor(x, y, fillColor);
            statRead++;
            statWrite++;
            x--;
        }
        if (x >= 0) statRead++;
        left = x + (ifCoh4 || x < 0);

        if (y < height - 1) updateStackLineStats(left, right, y+1, st);
        if (y > 0) updateStackLineStats(left, right, y-1, st);
    }
}