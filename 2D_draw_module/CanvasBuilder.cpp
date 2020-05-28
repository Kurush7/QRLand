//
// Created by kurush on 15.04.2020.
//

#include "CanvasBuilder.h"


void CanvasBuilder::enablePanel(bool enablePolygon, bool enableRect) {
    panel = new CanvasPanel(32, 64);
    panel->addButton(new QPushButton("line", this), [this](){this->is_building=true; state = LINE;
                                                                         setAlwaysStraightLines(false);});
    if (enableRect)
        panel->addButton(new QPushButton("rect", this), [this](){this->is_building=true; state = RECTANGLE;
                                                                         setAlwaysStraightLines(true);});
    if (enablePolygon) panel->addButton(new QPushButton("polygon", this), [this](){this->is_building=true; state = POLYGON;
                                                                            setAlwaysStraightLines(false);});
    panel->addButton(new QPushButton("clear", this), [this](){this->clearObjects(); emit this->cleared();});
    panel->setGeometry(this->width() - panel->width(), this->height()-panel->height(),
                       panel->width(), panel->height());
    panel->setParent(this);
}

void CanvasBuilder::keyPressEvent(QKeyEvent* event) {
    int key=event->key();
    if (key == Qt::Key_Escape) {
        clearObjects(true);
        return;
    }
    else if (key == Qt::Key_Return && state != LINE) {
        if (state == RECTANGLE) {
            if (buildPolyPts.size() < 3) {
                clearObjects(true);
                return;
            }
            else {
                if (buildPolyPts[1].x == buildPolyPts[0].x)
                    buildPolyPts.push_back(Point(buildPolyPts[3].x, buildPolyPts[0].y));
                else
                    buildPolyPts.push_back(Point(buildPolyPts[0].x, buildPolyPts[3].y));
            }
        }
        sendFigure();
        clearObjects(true);
    }
}

void CanvasBuilder::mousePressEvent(QMouseEvent* event) {
    PolyBuilder::mousePressEvent(event);
    if (state == LINE && buildPolyPts.size() == 2) {
        sendFigure();
        clearObjects(true);
        is_building = true;
    }
    else if (state == RECTANGLE && buildPolyPts.size() == 3) {
        if (buildPolyPts[0].x == buildPolyPts[2].x || buildPolyPts[0].y == buildPolyPts[2].y) {
            buildPolyPts.pop_back();
            deleteObject(buildPolyLines.back(), true);
            delete buildPolyLines.back();
            buildPolyLines.pop_back();
            return;
        }
        if (buildPolyPts[1].x == buildPolyPts[0].x)
            buildPolyPts.push_back(Point(buildPolyPts[3].x, buildPolyPts[0].y));
        else
            buildPolyPts.push_back(Point(buildPolyPts[0].x, buildPolyPts[3].y));
        sendFigure();
        clearObjects(true);
    }
}

void CanvasBuilder::sendFigure() {
    auto lc = lineColor? lineColor->getColor(): Qt::white;
    if (state == LINE) {
        emit sendLine(new Line(buildPolyPts[0], buildPolyPts[1], lc));
    }
    else if (state == RECTANGLE) {
        Point a = buildPolyPts[0].x < buildPolyPts[2].x? buildPolyPts[0]: buildPolyPts[2];
        Point b = buildPolyPts[0].x > buildPolyPts[2].x? buildPolyPts[0]: buildPolyPts[2];
        emit sendRect(new Rectangle(a, b, lc));
    }
    else if (state == POLYGON) {
        emit sendPolygon(new Polygon(buildPolyPts, lc));
    }
}