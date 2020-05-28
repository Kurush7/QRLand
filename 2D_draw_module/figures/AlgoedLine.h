//
// Created by kurush on 04.03.2020.
//

#ifndef KG_ALGOEDLINE_H
#define KG_ALGOEDLINE_H

#include "Primitives.h"
#include <cmath>

enum LineAlgos {
    DDA,
    BRESENHAM_FLOAT,
    BRESENHAM_INT,
    BRESENHAM_RESOLVE_STEPS,
    WU,
    CLASSIC
};

class AlgoedLine: public Line {
public:
    AlgoedLine();
    AlgoedLine(LineAlgos, Point, Point, LineStyle = LineStyle());
    AlgoedLine(LineAlgos,double, double, double, double, LineStyle = LineStyle());
    AlgoedLine(LineAlgos, Point, Point, QColor);
    AlgoedLine(const AlgoedLine &line);
    AlgoedLine(LineAlgos, const Line &line);
    virtual void draw(QPainter&, Transformer*, bool = true);

    LineAlgos alg;

private:
    void draw_classic(QPainter&, const Point&, const Point&);
    void draw_dda(QPainter&, const Point&, const Point&);
    void draw_bres_float(QPainter&, const Point&, const Point&);
    void draw_bres_int(QPainter&, const Point&, const Point&);
    void draw_bres_resolve_steps(QPainter&, const Point&, const Point&, float = 1);
    void draw_Wu(QPainter&, const Point&, const Point&, float = 1);
};

#endif //KG_ALGOEDLINE_H
