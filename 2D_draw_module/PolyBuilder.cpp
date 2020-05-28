//
// Created by kurush on 04.04.2020.
//

#include "PolyBuilder.h"



PolyBuilder::PolyBuilder(int w, int h, QWidget *parent,
                     QColor fill): Canvas(w, h, parent, fill) {
    setMouseTracking(true);
}

void PolyBuilder::paintEvent(QPaintEvent *event) {
    Canvas::paintEvent(event);
}

Point PolyBuilder::getRealMouseCoords(const Point& p0) {
    Point p;
    if (trans) {
        p = trans->fitBack(p0);
        Point min = trans->fitBack(Point(0,0));
        Point max = trans->fitBack(Point(width(), height()));
        swap(min.y, max.y);
        double kx = (p.x - min.x) / (max.x - min.x);
        double ky = (p.y - min.y) / (max.y - min.y);

        auto bords = viewer.getBorders();
        p.x = bords.x_min + kx * (bords.x_max - bords.x_min);
        p.y = bords.y_min + ky * (bords.y_max - bords.y_min);
    }
    return p;
}

Point PolyBuilder::cropAngle(Point base, Point p, int crop_cnt) {
    double div = M_PI*2 / crop_cnt;
    auto dx = p.x - base.x;
    auto dy = p.y - base.y;
    auto len = sign(dx) * sqrt(dx*dx+dy*dy);
    auto ang = atan(dy / dx);
    int cnt = round(ang/div);
    return Point(base.x + len*cos(div*cnt), base.y +  len*sin(div*cnt));
}

pair<bool, Point> PolyBuilder::alignToDirs(Point base, Point p) {
    if (lineDirs.empty())  return make_pair(false, p);
    double dir = (p.y - base.y) / (p.x - base.x);
    auto dx = p.x - base.x;
    auto dy = p.y - base.y;
    auto len = sign(dx) * sqrt(dx*dx+dy*dy);
    double best = *lower_bound(lineDirs.begin(), lineDirs.end(), dir);
    if (fabs((best - dir) / dir) < MIN_ALIGN_ERROR) {
        auto p1 = Point(base.x + len* cos(atan(best)), base.y + len*sin(atan(best)));
        return make_pair(true, p1);
    }
    return make_pair(false, p);
}

pair<bool, Point> PolyBuilder::alignToPoint(Point p) {
    if (points.empty())  return make_pair(false, p);
    QRPoint best(points[0]);
    double best_s = sqrt((best.x - p.x)*(best.x - p.x) + (best.y - p.y)*(best.y - p.y));
    for (auto x: points) {
        double s = sqrt((x.x - p.x)*(x.x - p.x) + (x.y - p.y)*(x.y - p.y));
        if (s < best_s) {
            best_s = s;
            best = x;
        }
    }
    if (best_s < MIN_ALIGN_POINT_DIST) {
        return make_pair(true, Point(best.x, best.y));
    }
    return make_pair(false, p);
}

pair<bool, Point> PolyBuilder::alignToEdge(Point p) {
    if (edges.empty())  return make_pair(false, p);
    QRPoint best(points[0]);
    double best_s = sqrt((best.x - p.x)*(best.x - p.x) + (best.y - p.y)*(best.y - p.y));
    for (auto x: edges) {
        Line l(x.a.x, x.a.y, x.b.x, x.b.y);
        double s = l.dist(p);
        if (s < best_s) {
            best_s = s;
            auto dir = l.getNormal();
            dir.x *= s, dir.y *= s;
            best = QRPoint(p.x + dir.x, p.y + dir.y);
            if (l.dist(Point(best.x, best.y)) > EPS)
                best.x -= 2*dir.x, best.y -= 2*dir.y;
        }
    }
    if (best_s < MIN_ALIGN_POINT_DIST) {
        return make_pair(true, Point(best.x, best.y));
    }
    return make_pair(false, p);
}

Point PolyBuilder::magicAlign(Point base, Point p) {
    if (alignMark) {
        Canvas::deleteObject(alignMark, true);
        alignMark = nullptr;
    }

    auto ans = alignToPoint(p);
    if (ans.first) {
        auto p1 = ans.second;
        alignMark = new Polygon(vector<Point>({Point(p1.x - 5, p1.y - 5),
                                               Point(p1.x - 5, p1.y + 5),
                                               Point(p1.x + 5, p1.y + 5),
                                               Point(p1.x + 5, p1.y - 5),}), activeColor);
        Canvas::addObject(alignMark, true);
        return ans.second;
    }

    ans = alignToEdge(p);
    if (ans.first) {
        auto p1 = ans.second;
        alignMark = new Polygon(vector<Point>({Point(p1.x - 5, p1.y - 5),
                                               Point(p1.x - 5, p1.y + 5),
                                               Point(p1.x + 5, p1.y - 5),
                                               Point(p1.x + 5, p1.y + 5),}), activeColor);
        Canvas::addObject(alignMark, true);
        return ans.second;
    }

    ans = alignToDirs(base, p);
    if (ans.first) {
        auto p1 = ans.second;
        alignMark = new Polygon(vector<Point>({Point(p1.x - 5, p1.y - 5),
                                               Point(p1.x - 5, p1.y + 5),
                                               Point(p1.x + 5, p1.y + 5),
                                               Point(p1.x + 5, p1.y - 5),
                                               Point(p1.x - 5, p1.y - 5),
                                               Point(p1.x - 5, p1.y + 5),
                                               Point(p1.x + 5, p1.y + 5),}), activeColor);
        Canvas::addObject(alignMark, true);
    }
    return ans.second;
}


void PolyBuilder::mousePressEvent(QMouseEvent* event) {
    if (markPointState) {
        markPointState = false;
        float x = event->pos().x();
        float y = event->pos().y();
        Point p = getRealMouseCoords(Point(x, y));
        emit sendMarkedPoint(Point(int(p.x), int(p.y)+1));  // +1... well, that's funny thing works
        if (activePoint) {
            deleteObject(activePoint, true);
            delete activePoint;
        }
        QColor c = fillColor->getColor();
        activePoint = new Point(int(p.x), int(p.y)+1, PointStyle(c, 1));
        addObject(activePoint, true);
        repaint();
        return;
    }
    if (alignMark) {
        Canvas::deleteObject(alignMark, true);
        alignMark = nullptr;
    }
    if (!is_building) {
        Canvas::mousePressEvent(event);
        return;
    }


    if (event->button() == Qt::LeftButton) {
        double x = event->pos().x();
        double y = event->pos().y();
        Point p = getRealMouseCoords(Point(x, y));
        if (buildPolyPts.size() && (event->modifiers() & Qt::ShiftModifier || alwaysStraightLines)) {
            p = cropAngle(buildPolyPts.back(), p, alwaysStraightLines?4:8);
        }
        if (event->modifiers() & Qt::CTRL ) {
            p = magicAlign(buildPolyPts.back(), p);
        }

        buildPolyPts.push_back(p);
        if (buildPolyPts.size() > 1) {
            auto color = activeColor;//lineColor? lineColor->getColor(): Qt::white;
            buildPolyLines.push_back(new AlgoedLine(LineAlgos::BRESENHAM_INT, buildPolyPts.back(), buildPolyPts[buildPolyPts.size() - 2], color));
            dynamicObjects.push_back(buildPolyLines.back());
            repaint();
        }
    }
    else if (event->button() == Qt::RightButton && !buildPolyPts.empty()) {
        buildPolyPts.pop_back();
        if (buildPolyPts.empty()) {
            deleteObject(activeLine, true);
            delete activeLine;
            activeLine = nullptr;
        }
        if(!buildPolyLines.empty()) {
            deleteObject(buildPolyLines.back(), true);
            delete buildPolyLines.back();
            buildPolyLines.pop_back();
        }
        update();
    }
}
void PolyBuilder::keyPressEvent(QKeyEvent* event) {
    int key=event->key();
    if (key == Qt::Key_Return || key == Qt::Key_Escape) {
        if (key == Qt::Key_Return) {
            if (filler) {
                for (auto line: buildPolyLines)
                    filler->addLine(new AlgoedLine(*line));
                auto color = activeColor;//lineColor? lineColor->getColor(): Qt::white;
                if (!alwaysStraightLines)
                    filler->addLine(new AlgoedLine(BRESENHAM_INT, buildPolyPts.back(), buildPolyPts.front(), color));
                else {
                    auto p1 = buildPolyPts.back(), p2 = buildPolyPts.front();
                    auto dx = p2.x - p1.x, dy = p2.y - p1.y;
                    filler->addLine(new AlgoedLine(BRESENHAM_INT, p1, Point(p1.x+dx, p1.y), color));
                    filler->addLine(new AlgoedLine(BRESENHAM_INT, Point(p1.x+dx, p1.y), p2, color));
                }
            }
        }
        clearObjects();
    }
}

void PolyBuilder::mouseMoveEvent(QMouseEvent *event) {
    if (!is_building) {
        Canvas::mouseMoveEvent(event);
        return;
    }
    if (buildPolyPts.empty())
        return;

    double x = event->pos().x();
    double y = event->pos().y();
    Point p = getRealMouseCoords(Point(x,y));
    if (event->modifiers() & Qt::CTRL) {
        p = magicAlign(buildPolyPts.back(), p);
    }
    else {
        if (alignMark) {
            Canvas::deleteObject(alignMark, true);
            alignMark = nullptr;
        }
        if (event->modifiers() & Qt::ShiftModifier || alwaysStraightLines) {
            p = cropAngle(buildPolyPts.back(), p, alwaysStraightLines?4:8);
        }
    }



    if (activeLine) {
        deleteObject(activeLine, true);
        delete activeLine;
    }
    auto color = activeColor;//lineColor? lineColor->getColor(): Qt::white;
    activeLine = new AlgoedLine(BRESENHAM_INT, p, buildPolyPts.back(), color);
    dynamicObjects.push_back(activeLine);
    update();
}

void PolyBuilder::clearObjects(bool dynamicsOnly) {
    for (auto line: buildPolyLines) {
        deleteObject(line, true);
        delete line;
    }
    buildPolyLines.clear();
    buildPolyPts.clear();
    if (activeLine) {
        deleteObject(activeLine, true);
        delete activeLine;
        activeLine = nullptr;
    }
    if (activePoint) {
        deleteObject(activePoint, true);
        delete activePoint;
        activePoint = nullptr;
    }
    is_building = false;
    if (filler)
        filler->clearLines();

    if (alignMark) {
        deleteObject(alignMark, true);
        alignMark = nullptr;
    }

    if (dynamicsOnly) {
        is_building = false;
        repaint();
    }
    else {
        Canvas::clearObjects();
        points.clear();
        edges.clear();
        lineDirs.clear();
        refillBg();
    }
}

void PolyBuilder::enablePanel() {
    panel = new CanvasPanel(32, 64);
    panel->addButton(new QPushButton("polynom", this), [this](){this->is_building=true;});
    panel->addButton(new QPushButton("clear", this), [this](){this->clearObjects();});
    panel->setGeometry(this->width() - panel->width(), this->height()-panel->height(),
            panel->width(), panel->height());
    panel->setParent(this);
}