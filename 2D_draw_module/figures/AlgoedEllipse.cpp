//
// Created by kurush on 04.03.2020.
//

#include "AlgoedEllipse.h"
#include <iostream>

AlgoedEllipse::AlgoedEllipse(CircleAlgos alg, Point c, double a, double b, LineStyle st)
        : alg(alg), center(c), A(a), B(b), style(st) {
    setStyle(st);
}

Borders AlgoedEllipse::getBorders() const {
    return Borders(center.x - A, center.x + A,
                   center.y - B, center.y + B);
}
void AlgoedEllipse::transform(Matrix<double> matr) {
    Matrix<double> mp(1, 3, vector<double>{center.x, center.y, 1});
    mp = product(mp, matr);

    center.x = mp[0][0];
    center.y = mp[0][1];
}

void AlgoedEllipse::draw(QPainter& p, Transformer* t, bool setStyle) {
    if (setStyle) {
        p.save();
        p.setPen(pen);
    }

    auto tc = t->fit(center);

    switch (alg) {
        case LIBRARY_CIRC:
            draw_lib(p, tc, A, B);
            break;
        case CLASSIC_CIRC:
            draw_classic(p, tc, A, B);
            break;
        case PARAMETRIC_CIRC:
            draw_parametric(p, tc, A, B);
            break;
        case BRESENHAM_CIRC:
            draw_bres(p, tc, A, B);
            break;
        case MIDPOINT_CIRC:
            draw_midp(p, tc, A, B);
            break;
    }

    if (setStyle) p.restore();
}

void AlgoedEllipse::draw_lib(QPainter &painter, const Point &c, double a, double b) {
    painter.drawEllipse(c.x - a, c.y - b, a*2, b*2);
}

void AlgoedEllipse::draw_classic(QPainter &painter, const Point &center, double A, double B) {
    int dx = 0, dy = Dround(B);
    int cx = Dround(center.x), cy = Dround(center.y);

    double A2 = A*A,B2 = B*B;
    double dyNext = B*sqrt(1-1/A), decY = dy - dyNext;  // k0 =
    while (dy >= 0) {
        painter.drawPoint(cx - dx, cy - dy);
        painter.drawPoint(cx - dx, cy + dy);
        painter.drawPoint(cx + dx, cy - dy);
        painter.drawPoint(cx + dx, cy + dy);

        if (decY < 1 - EPS) {
            dx++;
            dy = Dround(dyNext);
            decY = dyNext - B*sqrt(1 - (dx+1.)*(dx+1.)/A2);
            dyNext -= decY;
        }
        else {
            dy--;
            dx = Dround(A*sqrt(1 - (1.*dy)*dy/B2));
        }
    }
}

void AlgoedEllipse::draw_parametric(QPainter& painter, const Point& center, double A, double B) {
    int dx, dy;
    int cx = Dround(center.x), cy = Dround(center.y);

    double step = 1. / max(A,B);
    double t = 0., maxT = M_PI_2 + step/2;
    while (t < maxT) {
        dx = Dround(A * cos(t));
        dy = Dround(B * sin(t));

        painter.drawPoint(cx - dx, cy - dy);
        painter.drawPoint(cx - dx, cy + dy);
        painter.drawPoint(cx + dx, cy - dy);
        painter.drawPoint(cx + dx, cy + dy);
        t += step;
    }
}

void AlgoedEllipse::draw_bres(QPainter& painter, const Point& center, double a, double b) {
    int cx = Dround(center.x), cy = Dround(center.y);
    int dx = 0, dy = Dround(b);

    int A = Dround(a), B = dy;
    int D = A*A + B*B -2*B*A*A, d;
    A *= A, B *= B;

    while (dy >= 0) {
        painter.drawPoint(cx - dx, cy - dy);
        painter.drawPoint(cx - dx, cy + dy);
        painter.drawPoint(cx + dx, cy - dy);
        painter.drawPoint(cx + dx, cy + dy);

        if (D < 0) {
            d = 2*D + A*(2*dy - 1);
            dx++;
            if (d > 0) {
                dy--;
                D +=  B*(2*dx + 1) + A*(-2*dy + 1);
            }
            else
                D += B*(2*dx + 1);
        }
        else if (D > 0) {
            d = 2*D + B*(-2*dx - 1);
            dy--;
            if (d <= 0) {
                dx++;
                D +=  B*(2*dx + 1) + A*(-2*dy + 1);
            }
            else
                D += A*(-2*dy + 1);
        }
        else {
            dx++;
            dy--;
            D +=  B*(2*dx + 1) + A*(-2*dy + 1);
        }
    }
}

void AlgoedEllipse::draw_midp(QPainter& painter, const Point& center, double a, double b) {
    int cx = Dround(center.x), cy = Dround(center.y);
    int dx = 0, dy = Dround(b);

    int A = Dround(a*a), B = dy*dy;
    int fProb = A + 4*(B - A*dy);  //fProb = B*B + A*A/4 - A*A*B

    int segmentFlag = A*dy - B*dx;  // > 0 если верхний октант, иначе - нижний

    while (segmentFlag > 0) {
        painter.drawPoint(cx - dx, cy - dy);
        painter.drawPoint(cx - dx, cy + dy);
        painter.drawPoint(cx + dx, cy - dy);
        painter.drawPoint(cx + dx, cy + dy);

        dx++;
        if (fProb > 0) {
            dy--;
            segmentFlag -= A + B;
            fProb += 8 * (B * dx - A * dy) + 4 * B;
        } else {
            segmentFlag -= B;
            fProb += 4 * B * (2 * dx + 1);
        }
    }

    fProb += 3*(A-B) - 4*(B*dx+A*dy);
    while (dy >= 0) {
        painter.drawPoint(cx - dx, cy - dy);
        painter.drawPoint(cx - dx, cy + dy);
        painter.drawPoint(cx + dx, cy - dy);
        painter.drawPoint(cx + dx, cy + dy);

        dy--;
        if (fProb <= 0) {
            dx++;
            fProb += 8*(B*dx -A*dy) + 4*A;
        }
        else {
            fProb += 4*A*(-2*dy + 1);
        }
    }
}
