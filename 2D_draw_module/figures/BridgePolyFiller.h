//
// Created by kurush on 05.04.2020.
//

#ifndef KG_BRIDGEPOLYFILLER_H
#define KG_BRIDGEPOLYFILLER_H

#include "Primitives.h"
#include <QColor>
#include <QImage>

#include <chrono>

struct SimplePoint {
    SimplePoint(double x=0, double y=0): x(x), y(y) {}
    SimplePoint(Point a): x(a.x), y(a.y) {}
    double x, y;
};
bool operator <(const SimplePoint &a, const SimplePoint &b);

struct Edge {
    Edge(Point a0, Point b0): a(a0), b(b0) {}
    SimplePoint a, b;
};

enum VertexType {
    NOT_VISITED,
    UP,
    DOWN
};

struct timeResults {
    double time = 0;
    long long read = 0;
    long long write = 0;
    long long square = 0;
    long long used = 0;
};


struct BridgeDebugFill {
    map<SimplePoint, VertexType> vertices;
    std::vector<Edge> edges;
    int dx, dy, x, y, y0, e, i, cur_edge;
    bool yDirection;
    int mid;
    char sx, sy;
};

class PolyFiller: public DrawableObject {
public:
    PolyFiller(QImage *img): image(img) {}
    virtual void draw(QPainter&, Transformer*, bool = true);
    virtual void initDebug(Transformer *) = 0;
    void stopDebug() {debugging = false;}
    virtual void debugNext() = 0;
    virtual void debugLine() = 0;

    Borders getBorders() const {return Borders();}  // todo no info
    void transform(Matrix<double> matr) {}  // todo: doing nothing

    void setLineColor(QColor c) {lineColor = c;}
    void setBackColor(QColor c) {backColor = c;}
    void setFillColor(QColor c) {fillColor = c;}

    std::pair<double,double> getXBorders();

    void addLine(Line *l) {lines.push_back(l);}
    void clearLines() {
        for (auto line:lines)
            delete line;
        lines.clear();
        fill = false;
    }
    void setFill(bool b) {fill = b;}
    bool getFill() {return fill;}

    virtual timeResults measureTime(Transformer*);
    bool debugging = false;

protected:
    virtual int fillFigure(std::vector<Edge>&) = 0;

    virtual int fillFigureStats(std::vector<Edge>) = 0;
    // for statistics
    map<SimplePoint, bool> usedMap;
    int statRead = 0, statWrite = 0, statUsed=0;

    std::vector<Line*> lines;
    bool fill = false;
    QColor lineColor, backColor, fillColor;
    QImage *image;
};

class BridgePolyFiller: public PolyFiller {
public:
    BridgePolyFiller(QImage *img): PolyFiller(img){}
    virtual void initDebug(Transformer *);
    virtual void debugNext();
    virtual void debugLine();

    int getBridge() {return bridge;}
    int getAutoBridge();

    void setAutoBridging(bool a) {autoBridging = a;}
    void setBridge(int b) {bridge = b;}
private:
    inline void fillLine(int,int,int);
    inline void fillVertexes(std::vector<Edge>&);

    int fillFigure(std::vector<Edge>&) final;
    int fillFigureStats(std::vector<Edge>) final;

    void initDebugEdge(int);
    inline void fillLineStats(int,int,int);

    bool autoBridging = true;
    int bridge;

    BridgeDebugFill debug;
};


#endif //KG_BRIDGEPOLYFILLER_H
