//
// Created by kurush on 13.02.2020.
//

#ifndef KG_PRIMITIVES_H
#define KG_PRIMITIVES_H

#include <set>
#include <cmath>
#include <QPainter>
#include "../DrawStyles.h"
#include "Matrix.h"

using namespace std;


struct QRPoint {
    QRPoint(double x, double y): x(x), y(y) {}
    double x, y;
};
bool operator==(const QRPoint &p, const QRPoint &q);

struct QREdge {
    QREdge(double ax, double ay, double bx, double by): a(ax, ay), b(bx, by) {}
    double getDir() {return (b.y - a.y) / (b.x - a.x);}
    QRPoint a, b;
};
bool operator==(const QREdge &e, const QREdge &r);


const float EPS = 1e-5;
static inline int Dround(double x) {
    return round(x);
}
char sign(double x);
static inline char signInt(int x) {
    return x > 0? 1 : x < 0? -1 : 0;
}

const bool GLOBAL_DRAW_POINT_COORDS = false;

class Point;
class Transformer;

class Borders
{
public:
    double x_min, x_max, y_min, y_max;

    Borders(double xl, double xr, double yd, double yu): x_min(min(xl, xr)), x_max(max(xl, xr)),
                                                         y_min(min(yd, yu)), y_max(max(yd, yu)) {}
    Borders(): x_min(0), x_max(0), y_min(0), y_max(0) {}
    Borders(const Borders &b): x_min(b.x_min), x_max(b.x_max), y_min(b.y_min), y_max(b.y_max) {}

    void keepMax(Borders b);
};

class DrawableObject
{
public:
    DrawableObject(): pen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin) {}
    virtual ~DrawableObject() {}
    virtual Borders getBorders() const = 0;
    virtual std::vector<QRPoint> getPoints() {return std::vector<QRPoint>();}
    virtual std::vector<QREdge> getEdges() {return std::vector<QREdge>();}
    virtual void draw(QPainter&, Transformer*, bool) = 0;
    virtual void transform(Matrix<double> matr) = 0;

protected:
    QPen pen;
};

class Point: public DrawableObject
{
public:
    Point(double _x, double _y, int _n=-1, PointStyle st = PointStyle()): DrawableObject()
    {
        n = _n;
        x = _x;
        y = _y;
        setStyle(st);
    }
    Point(double _x, double _y, PointStyle st): DrawableObject()
    {
        n = -1;
        x = _x;
        y = _y;
        setStyle(st);
    }
    Point()
    {
        x = 0;
        y = 0;
        n = -1;
    }
    Point(const Point& p): DrawableObject(), n(p.n) {
        x = p.x;
        y = p.y;
        pen = p.pen;
    }

    void setStyle(PointStyle st)
    {
        pen = QPen(st.color, st.size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }

    Borders getBorders() const override;
    void draw(QPainter&, Transformer*, bool setStyle=true);
    virtual void transform(Matrix<double> matr) override;

    Point operator-(const Point &a) const{
        return Point(x - a.x, y - a.y);
    }
    bool operator<(const Point &a) const{
        return x < a.x || (x == a.x && y < a.y);
    }
    int n;
    double x, y;
};

class PointSet: public DrawableObject
{
public:
    PointSet(PointStyle st = PointStyle()): DrawableObject()
    {
        setStyle(st);
    }
    PointSet (const PointSet& s): DrawableObject() {
        points = s.points;
        pen = s.pen;
    }
    ~PointSet() = default;


    Borders getBorders() const override;
    bool add(Point &);
    bool has(Point &);
    bool erase(Point &);
    Point eraseByNum(int num);
    bool empty() {return points.empty();}
    void clear() {points.clear();}
    void draw(QPainter&, Transformer*, bool setStyle=true);
    int size() {return points.size();}

    virtual void transform(Matrix<double> matr) override;

    void setStyle(PointStyle st)
    {
        pen = QPen(st.color, st.size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }

    std::set<Point> points;

};


class Line: public DrawableObject
{
public:
    Point a, b;
    Line(): DrawableObject()
    {
        ka = a.y - b.y;
        kb = b.x - a.x;
        kc = - ka * a.x - kb * a.y;
        norm();
    }
    Line(Point a, Point b, LineStyle st = LineStyle()): DrawableObject(), a(a), b(b)
    {
        ka = a.y - b.y;
        kb = b.x - a.x;
        kc = - ka * a.x - kb * a.y;
        norm();
        setStyle(st);
    }
    Line(Point a, Point b, QColor color): DrawableObject(), a(a), b(b)
    {
        ka = a.y - b.y;
        kb = b.x - a.x;
        kc = - ka * a.x - kb * a.y;
        norm();
        auto st = LineStyle();
        st.color = color;
        setStyle(st);
    }
    Line(double xa, double ya, double xb, double yb, LineStyle st = LineStyle()): DrawableObject(), a(xa, ya), b(xb, yb)
    {
        ka = a.y - b.y;
        kb = b.x - a.x;
        kc = - ka * a.x - kb * a.y;
        norm();
        setStyle(st);
    }
    Line(const Line &l): a(l.a), b(l.b) {
        ka = a.y - b.y;
        kb = b.x - a.x;
        kc = - ka * a.x - kb * a.y;
        norm();
        pen = l.pen;
    }

    Point getNormal() const{
        return Point(ka, kb);
    }

    Borders getBorders() const override
    {
        Borders bord = a.getBorders();
        bord.keepMax(b.getBorders());
        return bord;
    }
    virtual void draw(QPainter&, Transformer*, bool setStyle=true);

    void setStyle(LineStyle st)
    {
        pen = QPen(st.color, st.width, st.style, Qt::RoundCap, Qt::RoundJoin);
    }
    void setStyle(QColor color)
    {
        pen = QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }

    virtual void transform(Matrix<double> matr) override;

    void norm();
    double dist(Point) const;

    double ka, kb, kc;
};


class Transformer
{
public:
    Transformer(Borders b = Borders()): borders(b) {}

    virtual Point fit(Point p)
    {
        return Point(p.x, borders.y_max - p.y, p.n);
    }
    virtual Point fitBack(Point p)
    {
        return Point(p.x, -p.y + borders.y_max, p.n);
    }

private:
    Borders borders;
};

class Scaler: public Transformer
{
public:
    Scaler(Borders from, Borders to, float keep): Transformer(to), from(from), to(to), keep(keep) {}

    Point fit(Point p);
    Point fitBack(Point p);

    Borders from, to;
    float keep;

};


double scalar(Point a, Point b);


#endif //KG_PRIMITIVES_H
