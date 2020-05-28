//
// Created by kurush on 04.03.2020.
//

#include "AlgoedCircle.h"
#include <iostream>

AlgoedCircle::AlgoedCircle(CircleAlgos alg, Point c, double rad, LineStyle st)
        : alg(alg), center(c), radius(rad), style(st) {
    setStyle(st);
}

Borders AlgoedCircle::getBorders() const {
    return Borders(center.x - radius, center.x + radius,
                   center.y - radius, center.y + radius);
}
void AlgoedCircle::transform(Matrix<double> matr) {
    Matrix<double> mp(1, 3, vector<double>{center.x, center.y, 1});
    mp = product(mp, matr);

    center.x = mp[0][0];
    center.y = mp[0][1];
}

void AlgoedCircle::draw(QPainter& p, Transformer* t, bool setStyle) {
    if (setStyle) {
        p.save();
        p.setPen(pen);
    }

    auto tc = t->fit(center);

    switch (alg) {
        case LIBRARY_CIRC:
            draw_lib(p, tc, radius);
            break;
        case CLASSIC_CIRC:
            draw_classic(p, tc, radius);
            break;
        case PARAMETRIC_CIRC:
            draw_parametric(p, tc, radius);
            break;
        case BRESENHAM_CIRC:
            draw_bres(p, tc, radius);
            break;
        case MIDPOINT_CIRC:
            draw_midp(p, tc, radius);
            break;
    }

    if (setStyle) p.restore();
}

void AlgoedCircle::draw_lib(QPainter &painter, const Point &a, double rad) {
    painter.drawEllipse(a.x - rad, a.y - rad, rad*2, rad*2);
}

void AlgoedCircle::draw_classic(QPainter &painter, const Point &center, double R) {
    int dx = 0, dy;

    int cx = Dround(center.x), cy = Dround(center.y);
    int dc = cx - cy;  // величина используется для корректного отражения октанта отн-но прямой в 45 градусов

    int R_2 = Dround(R / sqrt(2.));
    R *= R;
    while (dx <= R_2) {
        dy = Dround(sqrt(R - (1.*dx)*dx));

        painter.drawPoint(cx - dx, cy - dy); painter.drawPoint(cy - dy + dc, cx - dx - dc);
        painter.drawPoint(cx - dx, cy + dy); painter.drawPoint(cy + dy + dc, cx - dx - dc);
        painter.drawPoint(cx + dx, cy - dy); painter.drawPoint(cy - dy + dc, cx + dx - dc);
        painter.drawPoint(cx + dx, cy + dy); painter.drawPoint(cy + dy + dc, cx + dx - dc);

        dx++;
    }
}

void AlgoedCircle::draw_parametric(QPainter& painter, const Point& center, double R) {
    int dx, dy;

    int cx = Dround(center.x), cy = Dround(center.y);
    int dc = cx - cy;

    double step = 1. / R;
    double t = 0., maxT = M_PI_4 + step/2;
    while (t < maxT) {
        dx = Dround(R * cos(t));
        dy = Dround(R * sin(t));

        painter.drawPoint(cx - dx, cy - dy); painter.drawPoint(cy - dy + dc, cx - dx - dc);
        painter.drawPoint(cx - dx, cy + dy); painter.drawPoint(cy + dy + dc, cx - dx - dc);
        painter.drawPoint(cx + dx, cy - dy); painter.drawPoint(cy - dy + dc, cx + dx - dc);
        painter.drawPoint(cx + dx, cy + dy); painter.drawPoint(cy + dy + dc, cx + dx - dc);

        t += step;
    }
}

void AlgoedCircle::draw_bres(QPainter& painter, const Point& center, double R) {
    int cx = Dround(center.x), cy = Dround(center.y);
    int dx = 0, dy = Dround(R);
    int D = 2*(1 - dy), d;

    int dc = cx - cy;
    while (dy - dx >= 0) {
        painter.drawPoint(cx - dx, cy - dy); painter.drawPoint(cy - dy + dc, cx - dx - dc);
        painter.drawPoint(cx - dx, cy + dy); painter.drawPoint(cy + dy + dc, cx - dx - dc);
        painter.drawPoint(cx + dx, cy - dy); painter.drawPoint(cy - dy + dc, cx + dx - dc);
        painter.drawPoint(cx + dx, cy + dy); painter.drawPoint(cy + dy + dc, cx + dx - dc);

        if (D < 0) {
            d = 2*(D + dy) - 1;
            dx++;
            if (d > 0) {
                dy--;
                D += 2*(dx - dy + 1);
            }
            else
                D += 2*dx + 1;
        }
        else if (D > 0) {
            d = 2*(D - dx) - 1;
            dy--;
            if (d <= 0) {
                dx++;
                D += 2*(dx - dy + 1);
            }
            else
                D += -2*dy + 1;
        }
        else {
            dx++;
            dy--;
            D += 2*(dx - dy + 1);
        }
    }
}

void AlgoedCircle::draw_midp(QPainter& painter, const Point& center, double R) {
    int cx = Dround(center.x), cy = Dround(center.y);
    int dx = 0, dy = Dround(R);
    int fProb = 5 - 4*dy;

    int dc = Dround(center.x - center.y);
    while (dy - dx >= 0) {
        painter.drawPoint(cx - dx, cy - dy); painter.drawPoint(cy - dy + dc, cx - dx - dc);
        painter.drawPoint(cx - dx, cy + dy); painter.drawPoint(cy + dy + dc, cx - dx - dc);
        painter.drawPoint(cx + dx, cy - dy); painter.drawPoint(cy - dy + dc, cx + dx - dc);
        painter.drawPoint(cx + dx, cy + dy); painter.drawPoint(cy + dy + dc, cx + dx - dc);

        dx++;
        if (fProb > 0) {
            dy--;
            fProb += 8*(dx - dy) + 4;
        }
        else
            fProb += 8*dx + 4;
    }
}
