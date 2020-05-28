//
// Created by kurush on 04.03.2020.
//

#include "AlgoedLine.h"
#include <iostream>
using namespace std;

AlgoedLine::AlgoedLine(LineAlgos alg, Point a, Point b, LineStyle st)
: Line(a, b, st), alg(alg) {}

AlgoedLine::AlgoedLine(): Line(), alg(CLASSIC) {}

AlgoedLine::AlgoedLine(LineAlgos alg, double xa, double ya, double xb, double yb, LineStyle st): Line(xa, ya, xb, yb, st), alg(alg) {}

AlgoedLine::AlgoedLine(LineAlgos alg, Point a, Point b, QColor color)
: Line(a,b,color), alg(alg) {}

AlgoedLine::AlgoedLine(const AlgoedLine &line): Line(line), alg(line.alg) {}

AlgoedLine::AlgoedLine(LineAlgos alg, const Line &line): Line(line), alg(alg) {}

void AlgoedLine::draw(QPainter& p, Transformer* t, bool setStyle) {
    if (setStyle) {
        p.save();
        p.setPen(pen);
    }

    auto ta = t->fit(a);
    auto tb = t->fit(b);

    switch (alg) {
        case CLASSIC:
            draw_classic(p, ta, tb);
            break;
        case DDA:
            draw_dda(p, ta, tb);
            break;
        case BRESENHAM_FLOAT:
            draw_bres_float(p, ta, tb);
            break;
        case BRESENHAM_INT:
            draw_bres_int(p, ta, tb);
            break;
        case BRESENHAM_RESOLVE_STEPS:
            draw_bres_resolve_steps(p, ta, tb);
            break;
        case WU:
            draw_Wu(p, ta, tb);
            break;
    }

    if (setStyle) p.restore();
}

void AlgoedLine::draw_classic(QPainter &painter, const Point &a, const Point &b) {
    painter.drawLine(a.x, a.y, b.x, b.y);
}

void AlgoedLine::draw_dda(QPainter &painter, const Point &a, const Point &b) {
    float dx = b.x - a.x, dy = b.y - a.y;
    float Dx = fabs(dx), Dy = fabs(dy);
    float L =  Dx > Dy ? Dx : Dy;
    dx /= L;
    dy /= L;
    float x = a.x, y = a.y;

    int ll = ceil(L);
    cout << x << ' ' << y << " -> " << dx << ' ' << dy << '\n';
    for (int i = 0; i <= ll; ++i) {
       painter.drawPoint(Dround(x), Dround(y));
       cout << Dround(x) << ' ' << Dround(y) << '\n';
        x += dx, y += dy;
    }
    cout << "\n\n";
}

void AlgoedLine::draw_bres_float(QPainter &painter, const Point &a, const Point &b) {
    int x = Dround(a.x), y = Dround(a.y);
    float dx = b.x - a.x, dy = b.y - a.y;
    char sx = sign(dx), sy = sign(dy);
    dx = fabs(dx), dy = fabs(dy);

    bool swap = false;
    if (dx - dy < -EPS) {
        swap = true;
        float tmp = dx;
        dx = dy;
        dy = tmp;
    }

    float m = dy / dx;
    float e = (1. - (a.x - x))*m - 0.5;
    cout << e << "\n";
    for (int i = 0; i <= ceil(dx); ++i) {
        painter.drawPoint(x, y);
        if (e >= 0) {
            if (swap)
                x = x + sx;
            else
                y = y + sy;
            e -= 1;
        }
        if (swap)
            y = y + sy;
        else
            x = x + sx;
        e += m;
    }
}

void AlgoedLine::draw_bres_int(QPainter &painter, const Point &a, const Point &b) {
    int x = Dround(a.x), y = Dround(a.y);
    int dx = Dround(b.x) - x, dy = Dround(b.y) - y;
    char sx = signInt(dx), sy = signInt(dy);
    dx = fabs(dx), dy = fabs(dy);

    bool swap = false;
    if (dx < dy) {
        swap = true;
        int tmp = dx;
        dx = dy;
        dy = tmp;
    }

    int e = 2 * dy - dx;
    for (int i = 0; i <= dx; ++i) {
        painter.drawPoint(x, y);
        if (e >= 0) {
            if (swap)
                x = x + sx;
            else
                y = y + sy;
            e -= 2 * dx;
        }
        if (swap)
            y = y + sy;
        else
            x = x + sx;
        e += 2 * dy;
    }
}

void AlgoedLine::draw_bres_resolve_steps(QPainter &painter, const Point &a, const Point &b, float Imax) {
    float x = a.x, y = a.y;
    float dx = b.x - a.x, dy = b.y - a.y;
    char sx = sign(dx), sy = sign(dy);
    dx = fabs(dx), dy = fabs(dy);

    bool swap = false;
    if (dx - dy < -EPS) {
        swap = true;
        float tmp = dx;
        dx = dy;
        dy = tmp;
    }

    float m = Imax * dy/dx;
    float w = Imax - m;
    float e = Imax * 0.5;
    int ll = ceil(dx);
    for (int i = 0; i <= ll; ++i) {
        painter.setOpacity(e);
        painter.drawPoint(Dround(x), Dround(y));
        if (e > w) {
            if (swap)
                x = x + sx;
            else
                y = y + sy;
            e -= Imax;
        }
        if (swap)
            y = y + sy;
        else
            x = x + sx;
        e += m;
    }
}

void AlgoedLine::draw_Wu(QPainter &painter, const Point &a, const Point &b, float Imax) {
    float dx = b.x - a.x, dy = b.y - a.y;
    float Dx = fabs(dx), Dy = fabs(dy);
    float L =  Dx > Dy ? Dx : Dy;
    dx /= L;
    dy /= L;
    float x = a.x, y = a.y;
    bool swap = Dx - Dy < -EPS;

    float rx, ry, tmp;
    int ll = ceil(L);
    for (int i = 0; i <= ll; ++i) {
        rx = int(x), ry = int(y);
        tmp = Imax * (swap? (x - rx) : (y - ry));
        painter.setOpacity(Imax - tmp);
        painter.drawPoint(rx, ry);
        painter.setOpacity(tmp);
        if (swap)
            painter.drawPoint(rx + 1, ry);
        else
            painter.drawPoint(rx, ry + 1);

        y += dy;
        x += dx;
    }
}