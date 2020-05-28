//
// Created by kurush on 04.04.2020.
//

#ifndef KG_POLYBUILDER_H
#define KG_POLYBUILDER_H

#include "Canvas.h"
#include <cmath>
#include <algorithm>
#include "widgets/ColorEdit.h"
#include "figures/BridgePolyFiller.h"
#include "figures/Figures.h"


const double MIN_ALIGN_ERROR = 0.5;
const double MIN_ALIGN_POINT_DIST = 5;

class PolyBuilder: public Canvas {
    Q_OBJECT
public:
    explicit PolyBuilder(int w, int h, QWidget *parent = nullptr,
                    QColor fill = Qt::white);

    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event) {setFocus();}
    void mousePressEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent *event);

    virtual void enablePanel();
    virtual void clearObjects(bool dynamicsOnly = false);
    void setAlwaysStraightLines(bool b){alwaysStraightLines = b;}

    void markPoint(){ markPointState = true;}

    void bindPolyFiller(PolyFiller *f) {filler = f; }   // todo old color managing! do not use!
    void bindLineColor(ColorEdit* e) {lineColor = e;}
    void bindBackColor(ColorEdit* e) {backColor = e; connect(backColor, &QPushButton::clicked, [this](){fill= backColor->getColor();}); refillBg();}
    void bindFillColor(ColorEdit* e) {fillColor = e; connect(fillColor, &QPushButton::clicked,
            [this](){if (activePoint) activePoint->setStyle(PointStyle(fillColor->getColor(), 1));}); refillBg();}

    virtual void addObject(DrawableObject *obj, bool dynamic=false) {
        if (!dynamic) {
            for (auto x: obj->getEdges()) {
                edges.push_back(x);
                lineDirs.push_back((x.b.y - x.a.y) / (x.b.x - x.a.x));
            }
            sort(lineDirs.begin(), lineDirs.end());
            for (auto x: obj->getPoints())
                points.push_back(x);
        }
        Canvas::addObject(obj, dynamic);
    }
    virtual void deleteObject(DrawableObject *obj, bool dynamic=false) {
        if (!obj) return;
        if (!dynamic) {
            for (auto x: obj->getEdges()) {
                edges.erase(remove(edges.begin(), edges.end(), x),
                            edges.end());
                double dir = (x.b.y - x.a.y) / (x.b.x - x.a.x);     // todo if several has this dir!!!!!
                lineDirs.erase(remove(lineDirs.begin(), lineDirs.end(), dir),
                               lineDirs.end());
            }
            for (auto x: obj->getPoints())
                points.erase(remove(points.begin(), points.end(), x),
                             points.end());
        }
        Canvas::deleteObject(obj, dynamic);
    }

    void setActiveColor(QColor c) {activeColor = c;}


signals:
    void sendMarkedPoint(Point);

protected:
    bool markPointState = false;

    Point getRealMouseCoords(const Point&);
    Point cropAngle(Point base, Point p, int cnt=8);

    std::pair<bool, Point> alignToDirs(Point base, Point p);
    std::pair<bool, Point> alignToPoint(Point p);
    std::pair<bool, Point> alignToEdge(Point p);
    Point magicAlign(Point base, Point p);

    std::vector<Point> buildPolyPts;
    std::vector<AlgoedLine*> buildPolyLines;
    Line* activeLine = nullptr;
    Point* activePoint = nullptr;
    bool is_building;

    bool alwaysStraightLines = false;

    PolyFiller *filler = nullptr;
    ColorEdit *lineColor = nullptr, *backColor = nullptr, *fillColor=nullptr;
    QColor activeColor = Qt::white;

    std::vector<QREdge> edges;
    std::vector<double> lineDirs;
    std::vector<QRPoint> points;
    DrawableObject *alignMark = nullptr;
};


#endif //KG_POLYBUILDER_H
