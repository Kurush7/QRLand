//
// Created by kurush on 14.02.2020.
//

#ifndef KG_FIGURES_H
#define KG_FIGURES_H

#include "Primitives.h"
#include <vector>


#define POINT_DENSITY 0.005 // points per square unit
#define MIN_POINT_CNT 32.
#define RESCALE_COEFF  2

class Triangle: public DrawableObject {
public:
    Point a, b, c;

    Triangle(Point a, Point b, Point c,
            PointStyle p = PointStyle(), LineStyle l = LineStyle()): DrawableObject(), a(a), b(b), c(c)
    {
        setStyle(p, l);
    }
    Triangle(const Triangle &t): a(t.a), b(t.b), c(t.c), pen_line(t.pen_line)
    {
        pen = t.pen;
    }

    Borders getBorders() const override;
    void draw(QPainter&, Transformer*, bool=true);

    void setStyle(PointStyle st, LineStyle st_line)
    {
        pen = QPen(st.color, st.size, st_line.style, Qt::RoundCap, Qt::RoundJoin);
        pen_line = QPen(st_line.color, st_line.width, st_line.style, Qt::RoundCap, Qt::RoundJoin);
    }

    virtual void transform(Matrix<double> matr) override;

    double area();
    bool hasPoint(Point);

private:

    QPen pen_line;
};

class Rectangle: public DrawableObject {
public:
    Point lu, rd;
    double xl, xr, yu, yd;

    void adapt() {xl = lu.x, xr = rd.x, yu = lu.y, yd = rd.y;}

    Rectangle(Point lu, Point rd,
              PointStyle p = PointStyle(), LineStyle l = LineStyle()): DrawableObject(), lu(lu), rd(rd)
    {
        adapt();
        setStyle(p, l);
    }
    Rectangle(Point lu, Point rd,
              QColor color): DrawableObject(), lu(lu), rd(rd)
    {
        pen = QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        pen_line = QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        adapt();
    }
    Rectangle(const Rectangle &r): lu(r.lu), rd(r.rd), pen_line(r.pen_line)
    {
        pen = r.pen;
        adapt();
    }

    Borders getBorders() const override;
    void draw(QPainter&, Transformer*, bool=true);

    void setStyle(PointStyle st, LineStyle st_line)
    {
        pen = QPen(st.color, st.size, st_line.style, Qt::RoundCap, Qt::RoundJoin);
        pen_line = QPen(st_line.color, st_line.width, st_line.style, Qt::RoundCap, Qt::RoundJoin);
    }
    void setStyle(QColor color)
    {
        pen = QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        pen_line = QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }

    virtual void transform(Matrix<double> matr) override {} // todo no implementation

private:
    QPen pen_line;
};

class Polygon: public DrawableObject {
public:
    std::vector<Point> points;
    std::vector<Line> edges;

    void calculateEdges() {
        edges.resize(points.size());
        for (int i = 0; i < points.size(); ++i)
            edges[i] = Line(points[i], points[(i+1)%points.size()]);
    }

    Polygon(std::vector<Point> ps,
            PointStyle p = PointStyle(), LineStyle l = LineStyle()): DrawableObject(), points(ps) {
        setStyle(p, l);
        if (points.size() > 2) {
            auto p1 = points[0], p2 = points[1], p3 = points[2];
            auto v1 = p2 - p1, v2 = p3 - p2;
            if (v1.x*v2.y - v1.y*v2.x < 0) {
                reverse(points.begin(), points.end());
            }
        }
        calculateEdges();
    }
    Polygon(std::vector<Point> ps,
            QColor color): DrawableObject(), points(ps) {
        pen = QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        pen_line = QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        if (points.size() > 2) {
            auto p1 = points[0], p2 = points[1], p3 = points[2];
            auto v1 = p2 - p1, v2 = p3 - p2;
            if (v1.x*v2.y - v1.y*v2.x < 0) {
                reverse(points.begin(), points.end());
            }
        }
        calculateEdges();
    }
    Polygon(const Polygon &p): points(p.points), pen_line(p.pen_line) {
        pen = p.pen;
        calculateEdges();
    }

    Borders getBorders() const override;
    void draw(QPainter&, Transformer*, bool=true);

    void setStyle(PointStyle st, LineStyle st_line)
    {
        pen = QPen(st.color, st.size, st_line.style, Qt::RoundCap, Qt::RoundJoin);
        pen_line = QPen(st_line.color, st_line.width, st_line.style, Qt::RoundCap, Qt::RoundJoin);
    }
    void setStyle(QColor color)
    {
        pen = QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        pen_line = QPen(color, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }

    std::pair<bool, vector<Point>> checkConvexity() const;
    Point getNormal(int) const;
    double area() const;

    virtual void transform(Matrix<double> matr) override {} // todo no implementation

    virtual std::vector<QRPoint> getPoints() override;
    virtual std::vector<QREdge> getEdges() override;


private:
    QPen pen_line;
};

class Ellipse: public DrawableObject {
public:
    Ellipse(Point center, double width, double height, double rotation = 0,
            PointStyle = PointStyle(), LineStyle = LineStyle());
    Ellipse(const Ellipse &el);


    Borders getBorders() const override;
    void draw(QPainter&, Transformer*, bool=true);

    void setStyle(PointStyle st, LineStyle st_line)
    {
        pen = QPen(st.color, st.size, st_line.style, Qt::RoundCap, Qt::RoundJoin);
        pen_line = QPen(st_line.color, st_line.width, st_line.style, Qt::RoundCap, Qt::RoundJoin);
    }

    double atX(double x, bool isUp=true);
    virtual void transform(Matrix<double> matr) override;

    double predictArea(Matrix<double> = IDMatrix<double>(3));

private:
    void createEllipse(Point center, double width, double height, double rotation,
                        int new_point_cnt = -1);

    QPen pen_line;
    std::vector<Point> points;
    int point_cnt;
    double width, height;
    Point center;

    std::vector<Point> getPoints(double x, double aa, double bb);
    std::vector<Point>::iterator center_it;
};

#endif //KG_FIGURES_H
