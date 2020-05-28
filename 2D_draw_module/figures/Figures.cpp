//
// Created by kurush on 14.02.2020.
//

#include "Figures.h"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

void Rectangle::draw(QPainter &p, Transformer *t, bool setStyle)
{
    if (setStyle)
    {
        p.save();
        p.setPen(pen);
    }
    Point lut = t->fit(lu);
    Point rdt = t->fit(rd);

    //lu.draw(p, t, false);
    //rd.draw(p, t, false);
    //Point(lu.x, rd.y).draw(p, t, false);
    //Point(rd.x, lu.y).draw(p, t, false);
    lut.x = Dround(lut.x);
    lut.y = Dround(lut.y);
    rdt.x = Dround(rdt.x);
    rdt.y = Dround(rdt.y);

    if (setStyle) p.setPen(pen_line);
    p.setBackgroundMode(Qt::BGMode::OpaqueMode);
    p.drawRect(lut.x, lut.y, rdt.x-lut.x, fabs(lut.y-rdt.y));
    p.fillRect(lut.x+1, lut.y+1, rdt.x-lut.x-1, fabs(lut.y-rdt.y)-1, QColor(40, 40, 40));  //TODO CHEAT!!!!!
    p.setOpacity(0.1);
    p.fillRect(lut.x, lut.y, rdt.x-lut.x, fabs(lut.y-rdt.y), p.pen().color());

    if (setStyle) p.restore();
}

Borders Rectangle::getBorders() const
{
    return Borders(lu.x, rd.x, lu.y, rd.y);
}

void Polygon::draw(QPainter &p, Transformer *t, bool setStyle)
{
    if (setStyle)
    {
        p.save();
        p.setPen(pen);
    }
    for (auto point: points) {
        point = t->fit(point);
        p.drawPoint(point.x, point.y);
    }

    if (setStyle) p.setPen(pen_line);

    QVector<QPoint> pts;
    for (const auto & point : points) {
        auto pp = t->fit(point);
        pp.x = Dround(pp.x), pp.y = Dround(pp.y);
        pts.push_back(QPoint(pp.x, pp.y));
    }

    auto poly = QPolygon(pts);
    auto path = QPainterPath(QPoint(t->fit(points[0]).x, t->fit(points[0]).y));
    path.addPolygon(poly);

    p.fillPath(path, QColor(40,40,40));  // todo cheat

    QPen pen1 = pen;
    pen1.setWidth(2);
    p.setPen(pen1);
    for (int i = 0; i < points.size(); ++i) {
        auto p1 = t->fit(points[i]), p2 = t->fit(points[(i+1)%points.size()]);
        p1.x = Dround(p1.x), p1.y = Dround(p1.y);
        p2.x = Dround(p2.x), p2.y = Dround(p2.y);
        p.drawLine(p1.x, p1.y, p2.x, p2.y);
    }
    p.setPen(pen);

    p.setOpacity(0.1);
    p.fillPath(path, p.pen().color());

    if (setStyle) p.restore();
}

Borders Polygon::getBorders() const
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



double signSquare (Point a, Point b, Point c) {
    return a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
}

pair<bool, vector<Point>> Polygon::checkConvexity() const {
    int n = points.size();
    if (n == 1) return make_pair(false, points);
    vector<Point> a = points;
    sort(a.begin(), a.end());
    Point p1 = a[0],  p2 = a.back();
    vector<Point> up, down;
    up.push_back (p1);
    down.push_back (p1);
    for (size_t i=1; i<a.size(); ++i) {
        if (i==a.size()-1 || signSquare(p1, a[i], p2) < 0) {
            while (up.size()>=2 && signSquare(up[up.size()-2], up[up.size()-1], a[i]) >= 0)
                up.pop_back();
            up.push_back (a[i]);
        }
        if (i==a.size()-1 || signSquare(p1, a[i], p2) > 0) {
            while (down.size()>=2 && signSquare(down[down.size()-2], down[down.size()-1], a[i]) <= 0)
                down.pop_back();
            down.push_back (a[i]);
        }
    }
    a.clear();
    for (size_t i=0; i<up.size(); ++i)
        a.push_back (up[i]);
    for (size_t i=down.size()-2; i>0; --i)
        a.push_back (down[i]);
    return {a.size() == points.size(), a};
}

double Polygon::area() const {
    double area = 0;
    for (auto edge: edges)
        area += signSquare(edge.a, edge.b, Point(0,0));
    return fabs(area);
}

Point Polygon::getNormal(int i) const{
    Point normal = edges[i].getNormal();
    int i1 = (i+1) % edges.size();
    Point next_edge(edges[i1].b.x - edges[i1].a.x, edges[i1].b.y - edges[i1].a.y);
    if (scalar(normal, next_edge) > 0)
        return normal;
    return Point(-normal.x, -normal.y);
}

vector<QRPoint> Polygon::getPoints() {
    vector<QRPoint> ans;
    for (auto &p: points)
        ans.push_back(QRPoint(p.x, p.y));
    return ans;
}
vector<QREdge> Polygon::getEdges() {
    vector<QREdge> ans;
    for (auto &e: edges)
        ans.push_back(QREdge(e.a.x, e.a.y, e.b.x, e.b.y));
    return ans;
}


void Triangle::draw(QPainter &p, Transformer *t, bool setStyle)
{
    if (setStyle)
    {
        p.save();
        p.setPen(pen);
    }
    Point at = t->fit(a);
    Point bt = t->fit(b);
    Point ct = t->fit(c);

    a.draw(p, t, false);
    b.draw(p, t, false);
    c.draw(p, t, false);

    if (setStyle) p.setPen(pen_line);
    p.setBackgroundMode(Qt::BGMode::OpaqueMode);
    p.drawLine(at.x, at.y, bt.x, bt.y);
    p.drawLine(at.x, at.y, ct.x, ct.y);
    p.drawLine(bt.x, bt.y, ct.x, ct.y);

    if (setStyle) p.restore();
}

Borders Triangle::getBorders() const
{
    Borders bord = a.getBorders();
    bord.keepMax(b.getBorders());
    bord.keepMax(c.getBorders());
    return bord;
}

double Triangle::area()
{
    return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) / 2;
}

bool Triangle::hasPoint(Point p)
{
    double s1 = Triangle(a, b, p).area();
    double s2 = Triangle(b, c, p).area();
    double s3 = Triangle(c, a, p).area();

    double eps = 1e-9;
    if ((s1 > eps) && (s2 > eps) && (s3 > eps))
        return true;
    if ((s1 < -eps) && (s2 < -eps) && (s3 < -eps))
        return true;
    return false;
}


vector<Point> Ellipse::getPoints(double x, double aa, double bb)
{
    vector<Point> ans;
    Point p(x, sqrtf(bb - x*x*bb/aa));
    ans.push_back(p);
    p.y = -p.y;
    ans.push_back(p);
    return ans;
}

Point getPoint(double t, double a, double b)
{
    return Point(a*cos(t), b*sin(t));
}

Ellipse::Ellipse(Point center, double width, double height, double rotation,
        PointStyle p, LineStyle l): DrawableObject(), width(width), height(height), center(center)
{
    setStyle(p, l);
    createEllipse(center, width, height, rotation);
}

void Ellipse::createEllipse(Point center, double width, double height, double rotation, int new_point_cnt)
{
    // TODO ROTATION: ignored
    points.clear();

    if (new_point_cnt <= 0)
        point_cnt = max(MIN_POINT_CNT, width * height * POINT_DENSITY);
    else
        point_cnt = max(MIN_POINT_CNT, new_point_cnt +0.);
    auto t_step = 2*M_PI / point_cnt;

    Point pp;
    for (double t = 0; t < 2*M_PI; t += t_step)
    {
        pp = getPoint(t, width, height);
        points.push_back(Point(center.x + pp.x, center.y + pp.y));
    }
}


Ellipse::Ellipse(const Ellipse &el): DrawableObject(), width(el.width), height(el.height), center(el.center)
{
    pen = el.pen;
    pen_line = el.pen_line;
    points = el.points;
    point_cnt = el.point_cnt;
}

void Ellipse::draw(QPainter &p, Transformer *t, bool setStyle)
{
    if (setStyle)
    {
        p.save();
        p.setPen(pen);
    }

    if (setStyle) p.setPen(pen_line);
    p.setBackgroundMode(Qt::BGMode::OpaqueMode);

    for (int i = 0; i < points.size() - 1; ++i)
        Line(points[i], points[i+1]).draw(p, t, false);
    Line(points.back(), points[0]).draw(p, t, false);

    if (setStyle) p.restore();
}

Borders Ellipse::getBorders() const
{
    if (points.empty())
        return Borders();

    Borders bord = points[0].getBorders();
    for (auto p: points)
        bord.keepMax(p.getBorders());
    return bord;
}


bool cmpPointX(const Point &a, const Point &b) {
    return a.x < b.x;
}

double Ellipse::atX(double x, bool isUp) {
    auto res = getPoints(x - center.x, width*width, height*height);

    if (isUp)
        return res[0].y + center.y;
    else
        return res[1].y + center.y;
}


double len(Point a, Point b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) +
                   (a.y - b.y) * (a.y - b.y));
}

double Ellipse::predictArea(Matrix<double> matr)
{
    int shift = point_cnt/4;
    int maxArea = 0;
    int p1,p2,p3,p4, area;
    Point pp1,pp2,pp3,pp4;
    for (p1 = 0; p1 < point_cnt; p1 += point_cnt / 4)
    {
        p2 = (p1 + shift) % point_cnt;
        p3 = (p2 + shift) % point_cnt;
        p4 = (p3 + shift) % point_cnt;
        pp1 = points[p1]; pp1.transform(matr);
        pp2 = points[p2]; pp2.transform(matr);
        pp3 = points[p3]; pp3.transform(matr);
        pp4 = points[p4]; pp4.transform(matr);

        area = len(pp1, pp3) * len(pp2, pp4);

        maxArea = max(area, maxArea);
    }
    return maxArea;
}


void Triangle::transform(Matrix<double> matr) {
    a.transform(matr);
    b.transform(matr);
    c.transform(matr);
}


void Ellipse::transform(Matrix<double> matr) {

    double pred_area = predictArea(matr);
    double cur_area = predictArea();

    // VERY UNRELIABLE PART: WOKRS ONLY WHEN MANAGED BY 'DrawableManager' (figure must be
    // transformed only like this: default figure (with no rotation) is transformed by matrix; then
    // again defaults are set -> another matrix etc.
    if (pred_area / cur_area > RESCALE_COEFF)
    {
        createEllipse(center, width, height, 0,
                point_cnt * pred_area/cur_area);
    }

    for (int i = 0; i < points.size(); ++i)
        points[i].transform(matr);

}