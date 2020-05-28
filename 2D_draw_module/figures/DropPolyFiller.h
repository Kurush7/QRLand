//
// Created by kurush on 11.04.2020.
//

#ifndef KG_DROPPOLYFILLER_H
#define KG_DROPPOLYFILLER_H

#include "BridgePolyFiller.h"
#include <stack>

struct IntPoint {
    IntPoint(int x=0, int y=0): x(x), y(y) {}
    IntPoint(Point a): x(a.x), y(a.y) {}  // todo dround?
    int x, y;
};

struct DropDebugFill {
    std::vector<Edge> edges;
    std::stack<IntPoint> st;
    int x, y, i, cur_line, x0;
    int left, right;
    bool movingLeft;
};

class DropPolyFiller: public PolyFiller {
public:
    DropPolyFiller(QImage *img): PolyFiller(img){}
    virtual void draw(QPainter &, Transformer *, bool);
    virtual void initDebug(Transformer *);
    virtual void debugNext();
    virtual void debugLine();

    void setDrop(Point d) {drop = d;}
    void setCoherence(bool if4) {ifCoh4 = if4;}
private:
    int fillFigure(std::vector<Edge>&) final;
    inline void updateStackLine(int left, int right, int y, stack<IntPoint> &st);


    inline void updateStackLineStats(int, int, int, stack<IntPoint> &);
    int fillFigureStats(std::vector<Edge>) final;

    bool ifCoh4 = true;

    Point drop;
    IntPoint intDrop;
    DropDebugFill debug;
};

#endif //KG_DROPPOLYFILLER_H
