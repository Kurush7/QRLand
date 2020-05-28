//
// Created by kurush on 13.02.2020.
//

#include "Primitives.h"
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;

#define  POINT_BORDER_COEF 0.2
#define EPS 0.1

char sign(double x) {
    return x > EPS? 1 : x < -EPS? -1 : 0;
}

double scalar(Point a, Point b) {
    return a.x * b.x + a.y * b.y;
}

bool operator==(const QRPoint &p, const QRPoint &q) {return q.x == p.x && q.y == p.y;}
bool operator==(const QREdge &e, const QREdge &r) {return r.a == e.a && r.b == e.b;}

Point Scaler::fit(Point p)
{
    Point p1(0, 0, p.n);
    double coef = min((to.x_max - to.x_min) / (from.x_max - from.x_min),
                      (to.y_max - to.y_min) / (from.y_max - from.y_min));
    p1.x = to.x_min + keep*(to.x_max - to.x_min) +
               (p.x - from.x_min) * coef * (1 - 2*keep);
    p1.y = to.y_min + keep * (to.y_max - to.y_min) +
               (p.y - from.y_min) * coef * (1 - 2*keep);

    p1 = Transformer::fit(p1);
    return p1;
}
Point Scaler::fitBack(Point p)
{
    Point p1(0, 0, p.n);
    p = Transformer::fitBack(p);

    double coef = min((to.x_max - to.x_min) / (from.x_max - from.x_min),
                      (to.y_max - to.y_min) / (from.y_max - from.y_min));

    // some bugs encountered: incorrect marker show (big X on the left side when standart axis...)
    // problems with keeping
    p1.x = ((p.x -to.x_min - keep*(to.x_max - to.x_min)) / ((1 - 2*keep)) / (coef) + from.x_min);
    p1.y = ((p.y -to.y_min - keep*(to.y_max - to.y_min)) / ((1 - 2*keep)) / (coef) + from.y_min);

    return p1;
}

void Borders::keepMax(Borders b)
{
    x_min = min(x_min, b.x_min);
    y_min = min(y_min, b.y_min);
    x_max = max(x_max, b.x_max);
    y_max = max(y_max, b.y_max);
}

void Point::draw(QPainter &painter, Transformer *transformer, bool setStyle)
{
    if (setStyle) {
        painter.save();
        painter.setPen(pen);
    }

    auto p = transformer->fit(*this);
    painter.drawPoint(Dround(p.x), Dround(p.y));

    auto xx = floor(x*100 )/100;
    auto yy = floor(y*100 )/100;

    QString text = "";
    if (GLOBAL_DRAW_POINT_COORDS)
        text = "(" + QString::number(xx) + ", " + QString::number(yy) + ")";
    if (n >= 0)
        text = QString::number(n) + " " + text;
    painter.drawText(p.x + painter.pen().width(), p.y - painter.pen().width(), text);

    if (setStyle) painter.restore();
}
void Line::draw(QPainter &p, Transformer *t, bool setStyle)
{
    if (setStyle) {
        p.save();
        p.setPen(pen);
    }
    auto pa = t->fit(a);
    auto pb = t->fit(b);
    p.setBackgroundMode(Qt::BGMode::OpaqueMode);
    p.drawLine(Dround(pa.x), Dround(pa.y), Dround(pb.x), Dround(pb.y));

    if (setStyle) p.restore();
}
void PointSet::draw(QPainter &painter, Transformer *transformer, bool setStyle)
{
    if (setStyle)
    {
        painter.save();
        painter.setPen(pen);
    }

    for (auto p: points)
        p.draw(painter, transformer, false);

    if (setStyle) painter.restore();
}


Borders Point::getBorders() const
{
    return Borders(x - EPS * POINT_BORDER_COEF, x + EPS* POINT_BORDER_COEF,
                   y - EPS * POINT_BORDER_COEF, y + EPS * POINT_BORDER_COEF);
}
Borders PointSet::getBorders() const
{
    if (points.empty())
        return Borders();
    Borders res = points.begin()->getBorders();
    for (auto p: points)
    {
        res.keepMax(p.getBorders());
    }

    return res;
}

bool PointSet::add(Point &p)
{
    if (has(p))
        return false;

    points.insert(p);
    return true;
}
bool PointSet::has(Point &p)
{
    return points.find(p) != points.end();
}
bool PointSet::erase(Point &p)
{
    if (!has(p))
        return false;

    points.erase(points.find(p));
    return true;
}
Point PointSet::eraseByNum(int num)
{
    for (auto p: points)
    {
        if (p.n == num) {
            points.erase(points.find(p));
            return p;
        }
    }
    return Point(0,0,-1);
}


void Line::norm() {
    double z = sqrt (ka*ka + kb*kb);
    if (abs(z) > 1e-5)
        ka /= z,  kb /= z,  kc /= z;
}
double Line::dist(Point p) const{
    return fabs(ka * p.x + kb * p.y + kc);
}



void Point::transform(Matrix<double> matr)
{
    Matrix<double> mp(1, 3, vector<double>{x, y, 1});
    mp = product(mp, matr);

    x = mp[0][0];
    y = mp[0][1];
}
void PointSet::transform(Matrix<double> matr)
{
    set<Point> new_points;
    for (auto p: points)
    {
        p.transform(matr);
        new_points.insert(p);
    }
    points = new_points;
}
void Line::transform(Matrix<double> matr)
{
    a.transform(matr);
    b.transform(matr);
}