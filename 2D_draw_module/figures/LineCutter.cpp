//
// Created by kurush on 15.04.2020.
//

#include "LineCutter.h"

using namespace std;

enum Visibility {
    VISIBLE, INVISIBLE, POTENTIAL
};

inline void getCode(const Point a, double rect[4], char &code) {
    code = 0;
    if (a.x < rect[0]) code += 1;
    if (a.x > rect[1]) code += 2;
    if (a.y < rect[2]) code += 4;
    if (a.y > rect[3]) code += 8;
}

inline Visibility isVisible(int codeA, int codeB) {
    Visibility v = POTENTIAL;
    if (codeA == 0 && codeB == 0)
        v = VISIBLE;
    else if ((codeA & codeB) != 0)
        v = INVISIBLE;
    return v;
}

bool cutLine(const AlgoedLine &line, const Rectangle &rt, AlgoedLine &cut) {
    AlgoedLine ln(line);    // копирование исходного отрезка, чтобы не изменять входные данные
    double rect[4] = {rt.xl, rt.xr, rt.yd, rt.yu}; // 0 – слева, 1 – справа, 2 – снизу, 3 - сверху
    char codeA, codeB, orientation = 0;
    double m = 0;
    if (abs(ln.a.x - ln.b.x) < EPS) orientation = 1;    // вертикальный
    else if (abs(ln.a.y - ln.b.y) < EPS) orientation = -1;  // горизонтальный
    else {
        m = (ln.b.y - ln.a.y) / (ln.b.x - ln.a.x);
    }

    getCode(ln.a, rect, codeA);
    getCode(ln.b, rect, codeB);
    Visibility v = isVisible(codeA, codeB);
    if (v == VISIBLE) {    // тривиально видимый отрезок
        cut = ln;
        return true;
    }
    else if (v == INVISIBLE) return false;  // тривиально невидимый

    for (int i = 0; i < 4; ++i) {
        if ((codeA & (1 << i)) == (codeB & (1 << i))) continue; // весь отрезок находится по одну сторону грани
        else if ((codeA & (1 << i)) == 0) {     // первая точка видима - надо поменять точки местами
            swap(ln.a, ln.b);   // обмен значений переменных
            swap(codeA, codeB);
        }

        if (i < 2){
            if (orientation != -1) ln.a.y = m * (rect[i] - ln.a.x) + ln.a.y;
            ln.a.x = rect[i];
        }
        else {
            if (orientation != 1) ln.a.x = (1/m) * (rect[i] - ln.a.y) + ln.a.x;
            ln.a.y = rect[i];
        }

        getCode(ln.a, rect, codeA);
        v = isVisible(codeA, codeB);
        if (v == VISIBLE) {
            cut = ln;
            return true;
        }
        else if (v == INVISIBLE) return false;
    }
    return 0;
}



bool cutLine(const AlgoedLine &line, const Polygon &poly, AlgoedLine &cut) {
    Point W, norm, D = line.b - line.a;
    double t_l = 0, t_r = 1, Ds, Ws;
    int n = poly.edges.size();

    for (int i = 0; i < n; ++i) {
        W = line.a - poly.edges[i].a;
        norm = poly.getNormal(i);
        Ds = scalar(D, norm);
        Ws = scalar(W, norm);

        if (abs(Ds) < EPS) {
            if (Ws < 0) return false;
            continue;
        }

        double t = - Ws / Ds;
        if (Ds > EPS) {
            if (t > 1 + EPS) return false;
            t_l = max(t, t_l);
        }
        else {
            if (t < -EPS) return false;
            t_r = min(t, t_r);
        }
    }

    if (t_l <= t_r + EPS) {
        Point p1(line.a.x + D.x * t_l, line.a.y + D.y * t_l);
        Point p2(line.a.x + D.x * t_r, line.a.y + D.y * t_r);
        cut = AlgoedLine(cut.alg, p1, p2);
        return true;
    }
    return false;
}