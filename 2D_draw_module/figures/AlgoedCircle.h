//
// Created by kurush on 24.03.2020.
//

#ifndef KG_ALGOEDCIRCLE_H
#define KG_ALGOEDCIRCLE_H

#include "Primitives.h"
#include <cmath>

enum CircleAlgos {
    CLASSIC_CIRC,
    PARAMETRIC_CIRC,
    BRESENHAM_CIRC,
    MIDPOINT_CIRC,
    LIBRARY_CIRC
};

class AlgoedCircle: public DrawableObject {
public:
    AlgoedCircle(CircleAlgos, Point, double, LineStyle = LineStyle());
    virtual void draw(QPainter&, Transformer*, bool = true);

    Borders getBorders() const;
    void transform(Matrix<double> matr);  // todo: not properly made: center transform only!!!!!

private:
    Point center;
    double radius;
    CircleAlgos alg;
    LineStyle style;

    void draw_classic(QPainter&, const Point&, double);
    void draw_parametric(QPainter&, const Point&, double);
    void draw_bres(QPainter&, const Point&, double);
    void draw_midp(QPainter&, const Point&, double);
    void draw_lib(QPainter&, const Point&, double);

    void setStyle(LineStyle st)
    {
        pen = QPen(st.color, st.width, st.style, Qt::RoundCap, Qt::RoundJoin);
    }
};


#endif //KG_ALGOEDCIRCLE_H
